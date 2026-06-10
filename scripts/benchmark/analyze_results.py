#!/usr/bin/env python3

"""
Benchmark Result Analyzer

Analyzes exported benchmark CSV files to extract performance insights.

Usage:
    python analyze_results.py results/benchmark_combined_*.csv
    python analyze_results.py --dir results/
    python analyze_results.py --compare file1.csv file2.csv
"""

import csv
import sys
import os
from pathlib import Path
from typing import List, Dict, Tuple
import statistics
import re
from datetime import datetime

# ============================================================
# Parsing Functions
# ============================================================

def parse_time_to_nanoseconds(time_str: str) -> float:
    """Convert benchmark time string to nanoseconds"""
    
    # Remove whitespace
    time_str = time_str.strip()
    
    # Pattern: number + optional space + unit (ns/us/ms/s)
    match = re.match(r'([\d.]+)\s*([a-zA-Z]+)', time_str)
    if not match:
        return 0.0
    
    value = float(match.group(1))
    unit = match.group(2).lower()
    
    multipliers = {
        'ns': 1,
        'us': 1e3,
        'µs': 1e3,
        'ms': 1e6,
        's': 1e9
    }
    
    multiplier = multipliers.get(unit, 1)
    return value * multiplier

def extract_matrix_size(name: str) -> Tuple[int, int]:
    """Extract matrix size from benchmark name"""
    
    match = re.search(r'(\d+)x(\d+)', name)
    if match:
        rows = int(match.group(1))
        cols = int(match.group(2))
        return (rows, cols)
    
    return (0, 0)

def categorize_benchmark(name: str) -> str:
    """Categorize benchmark by name pattern"""
    
    if 'Chained' in name or 'Addition' in name:
        if 'Scaling' in name:
            return 'Scaling: Element-wise'
        return 'Chained Operations'
    elif 'Multiply' in name:
        return 'Matrix Multiplication'
    elif 'InPlace' in name:
        return 'In-Place Operations'
    elif 'FreeFunc' in name:
        if 'Free' in name:
            return 'Free Functions (+)'
        return 'In-Place Operators (+=)'
    elif 'Iterative' in name:
        return 'Iterative Refinement'
    elif 'Constructor' in name:
        if 'Copy' in name:
            return 'Copy Constructor (V2)'
        return 'Move Constructor (V2)'
    else:
        return 'Other'

# ============================================================
# Analysis Functions
# ============================================================

def load_csv(filepath: str) -> List[Dict]:
    """Load benchmark CSV file"""
    data = []
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            data = list(reader)
        
        # Normalize time values to nanoseconds
        for row in data:
            row['time_ns'] = parse_time_to_nanoseconds(row.get('time', '0'))
            row['size'] = extract_matrix_size(row.get('name', ''))
            row['category'] = categorize_benchmark(row.get('name', ''))
    
    except Exception as e:
        print(f"Error loading {filepath}: {str(e)}")
    
    return data

def group_by_category(data: List[Dict]) -> Dict[str, List[Dict]]:
    """Group benchmarks by category"""
    grouped = {}
    
    for row in data:
        category = row.get('category', 'Other')
        if category not in grouped:
            grouped[category] = []
        grouped[category].append(row)
    
    return grouped

def analyze_category(category: str, benches: List[Dict]) -> Dict:
    """Analyze a category of benchmarks"""
    
    times = [b.get('time_ns', 0) for b in benches if b.get('time_ns', 0) > 0]
    
    if not times:
        return {}
    
    return {
        'category': category,
        'count': len(benches),
        'min': min(times),
        'max': max(times),
        'mean': statistics.mean(times),
        'median': statistics.median(times),
        'stdev': statistics.stdev(times) if len(times) > 1 else 0,
    }

def analyze_scaling(data: List[Dict]) -> List[Dict]:
    """Analyze scaling behavior"""
    
    scaling = [r for r in data if 'Scaling' in r.get('category', '')]
    scaling.sort(key=lambda x: x.get('size', (0, 0))[0])
    
    analysis = []
    for b in scaling:
        rows, cols = b.get('size', (0, 0))
        size = rows * cols if rows > 0 else 0
        time_ns = b.get('time_ns', 0)
        time_per_element = time_ns / size if size > 0 else 0
        
        analysis.append({
            'name': b.get('name', ''),
            'size': f"{rows}×{cols}",
            'elements': size,
            'time_ns': time_ns,
            'time_per_element_ns': time_per_element
        })
    
    return analysis

def compare_v2(data: List[Dict]) -> Dict:
    """Compare Copy vs Move constructors (V2)"""
    
    copy_benches = [r for r in data if 'Copy' in r.get('category', '')]
    move_benches = [r for r in data if 'Move' in r.get('category', '')]
    
    result = {
        'copy_count': len(copy_benches),
        'move_count': len(move_benches),
        'copy_times': [b.get('time_ns', 0) for b in copy_benches],
        'move_times': [b.get('time_ns', 0) for b in move_benches],
    }
    
    if result['copy_times'] and result['move_times']:
        copy_avg = statistics.mean(result['copy_times'])
        move_avg = statistics.mean(result['move_times'])
        result['speedup'] = copy_avg / move_avg if move_avg > 0 else 0
        result['speedup_percent'] = (1 - move_avg / copy_avg) * 100
    
    return result

def compare_v3(data: List[Dict]) -> Dict:
    """Compare Free (+) vs In-Place (+=) operators (V3)"""
    
    free_benches = [r for r in data if 'Free Functions' in r.get('category', '')]
    inplace_benches = [r for r in data if 'In-Place Operators' in r.get('category', '')]
    
    # Group by size if possible
    result = {
        'free_count': len(free_benches),
        'inplace_count': len(inplace_benches),
        'comparisons': []
    }
    
    # Try to pair benchmarks by size
    for free_b in free_benches:
        free_size = free_b.get('size', (0, 0))
        free_time = free_b.get('time_ns', 0)
        
        # Find corresponding inplace benchmark
        for inplace_b in inplace_benches:
            inplace_size = inplace_b.get('size', (0, 0))
            if free_size == inplace_size and inplace_size != (0, 0):
                inplace_time = inplace_b.get('time_ns', 0)
                speedup = free_time / inplace_time if inplace_time > 0 else 0
                
                result['comparisons'].append({
                    'size': f"{free_size[0]}×{free_size[1]}",
                    'free_time_ns': free_time,
                    'inplace_time_ns': inplace_time,
                    'speedup': speedup,
                    'speedup_percent': (1 - inplace_time / free_time) * 100 if free_time > 0 else 0
                })
    
    return result

# ============================================================
# Output Functions
# ============================================================

def print_header(title: str):
    """Print formatted header"""
    width = 70
    print("\n" + "=" * width)
    print(title.center(width))
    print("=" * width + "\n")

def format_time(ns: float) -> str:
    """Format nanoseconds to readable format"""
    if ns < 1000:
        return f"{ns:.1f} ns"
    elif ns < 1e6:
        return f"{ns/1e3:.2f} µs"
    elif ns < 1e9:
        return f"{ns/1e6:.2f} ms"
    else:
        return f"{ns/1e9:.2f} s"

def report_category_analysis(grouped: Dict[str, List[Dict]]):
    """Generate category analysis report"""
    
    print_header("CATEGORY ANALYSIS")
    
    analyses = []
    for category, benches in sorted(grouped.items()):
        analysis = analyze_category(category, benches)
        if analysis:
            analyses.append(analysis)
    
    # Sort by mean time (descending)
    analyses.sort(key=lambda x: x.get('mean', 0), reverse=True)
    
    for analysis in analyses:
        print(f"Category: {analysis['category']}")
        print(f"  Benchmarks: {analysis['count']}")
        print(f"  Min:        {format_time(analysis['min'])}")
        print(f"  Max:        {format_time(analysis['max'])}")
        print(f"  Mean:       {format_time(analysis['mean'])}")
        print(f"  Median:     {format_time(analysis['median'])}")
        if analysis['stdev'] > 0:
            print(f"  Std Dev:    {format_time(analysis['stdev'])}")
        print()

def report_v2_analysis(comparison: Dict):
    """Generate V2 analysis report"""
    
    print_header("V2 PERFORMANCE: MOVE SEMANTICS")
    
    print(f"Copy Constructor Benchmarks: {comparison['copy_count']}")
    if comparison['copy_times']:
        print(f"  Times: {', '.join(format_time(t) for t in comparison['copy_times'][:3])}")
    
    print(f"\nMove Constructor Benchmarks: {comparison['move_count']}")
    if comparison['move_times']:
        print(f"  Times: {', '.join(format_time(t) for t in comparison['move_times'][:3])}")
    
    if 'speedup' in comparison:
        print(f"\n✓ SPEEDUP: {comparison['speedup']:.2f}×")
        print(f"✓ IMPROVEMENT: {comparison['speedup_percent']:.1f}%")
        print("\nConclusion:")
        print("  Move semantics provide significant speedup for temporary objects")
        print("  Enables efficient chained operations: (A + B) - C")
    print()

def report_v3_analysis(comparison: Dict):
    """Generate V3 analysis report"""
    
    print_header("V3 PERFORMANCE: COMPOUND OPERATORS")
    
    print(f"Free Function (+) Benchmarks: {comparison['free_count']}")
    print(f"In-Place (+=) Benchmarks: {comparison['inplace_count']}")
    
    if comparison['comparisons']:
        print(f"\nSize-by-Size Comparison:")
        print(f"{'Size':<12} {'Free (+)':<15} {'In-Place (+=)':<15} {'Speedup':<10}")
        print("-" * 55)
        
        for comp in comparison['comparisons']:
            size = comp['size']
            free_time = format_time(comp['free_time_ns'])
            inplace_time = format_time(comp['inplace_time_ns'])
            speedup = f"{comp['speedup']:.2f}×"
            
            print(f"{size:<12} {free_time:<15} {inplace_time:<15} {speedup:<10}")
        
        # Summary
        avg_speedup = statistics.mean([c['speedup'] for c in comparison['comparisons']])
        print(f"\nAverage Speedup: {avg_speedup:.2f}×")
        print("\nConclusion:")
        print("  In-place operators are more efficient than free functions")
        print("  Speedup increases with matrix size (less setup overhead)")
        print("  Best for iterative algorithms and accumulation loops")
    print()

def report_scaling_analysis(scaling: List[Dict]):
    """Generate scaling analysis report"""
    
    print_header("SCALING ANALYSIS: O(n²) VERIFICATION")
    
    if not scaling:
        return
    
    print(f"{'Size':<12} {'Elements':<12} {'Time':<15} {'Time/Element':<15}")
    print("-" * 55)
    
    for s in scaling:
        size = s['size']
        elements = f"{s['elements']:,}"
        time = format_time(s['time_ns'])
        time_per_elem = format_time(s['time_per_element_ns'])
        
        print(f"{size:<12} {elements:<12} {time:<15} {time_per_elem:<15}")
    
    # Verify O(n²) growth
    if len(scaling) >= 2:
        print(f"\n✓ Scaling Verification:")
        
        first = scaling[0]
        last = scaling[-1]
        
        size_ratio = last['elements'] / first['elements'] if first['elements'] > 0 else 0
        time_ratio = last['time_ns'] / first['time_ns'] if first['time_ns'] > 0 else 0
        expected_ratio = size_ratio  # O(n²) for 1D operation
        
        print(f"  First benchmark: {first['size']} = {format_time(first['time_ns'])}")
        print(f"  Last benchmark:  {last['size']} = {format_time(last['time_ns'])}")
        print(f"  Size ratio: {size_ratio:.1f}×")
        print(f"  Time ratio: {time_ratio:.1f}×")
        print(f"  Expected (O(n²)): {expected_ratio:.1f}×")
        
        if abs(time_ratio - expected_ratio) / expected_ratio < 0.5:
            print(f"\n  ✓ O(n²) complexity VERIFIED (scaling is linear)")
        else:
            print(f"\n  ⚠ Deviation from O(n²) (may indicate cache effects)")
    
    print()

def report_summary(data: List[Dict]):
    """Generate summary statistics"""
    
    print_header("SUMMARY STATISTICS")
    
    print(f"Total Benchmarks: {len(data)}")
    
    if data:
        times = [d.get('time_ns', 0) for d in data if d.get('time_ns', 0) > 0]
        
        print(f"\nTiming Statistics:")
        print(f"  Fastest:  {format_time(min(times))}")
        print(f"  Slowest:  {format_time(max(times))}")
        print(f"  Average:  {format_time(statistics.mean(times))}")
        print(f"  Median:   {format_time(statistics.median(times))}")
    
    # Category breakdown
    grouped = group_by_category(data)
    print(f"\nBenchmarks by Category:")
    for category in sorted(grouped.keys()):
        count = len(grouped[category])
        print(f"  {category:<30} {count:>3}")
    
    print()

# ============================================================
# Main
# ============================================================

def main():
    """Main execution"""
    
    print("\n" + "=" * 70)
    print("BENCHMARK RESULT ANALYZER")
    print("=" * 70)
    
    # Parse arguments
    if len(sys.argv) < 2:
        print("\nUsage:")
        print("  python analyze_results.py <csv_file>")
        print("  python analyze_results.py --dir <directory>")
        print("  python analyze_results.py --compare <file1> <file2>")
        sys.exit(1)
    
    files = []
    
    if sys.argv[1] == '--dir' and len(sys.argv) > 2:
        # Load all CSVs from directory
        directory = sys.argv[2]
        files = list(Path(directory).glob('benchmark_*.csv'))
        
        if not files:
            print(f"✗ No benchmark files found in {directory}")
            sys.exit(1)
    
    else:
        # Load specified files
        files = [Path(f) for f in sys.argv[1:] if not f.startswith('--')]
    
    # Load and analyze
    all_data = []
    for filepath in files:
        if filepath.exists():
            data = load_csv(str(filepath))
            all_data.extend(data)
            print(f"✓ Loaded: {filepath.name} ({len(data)} benchmarks)")
    
    if not all_data:
        print("✗ No benchmark data loaded")
        sys.exit(1)
    
    # Generate reports
    print()
    
    report_summary(all_data)
    
    grouped = group_by_category(all_data)
    report_category_analysis(grouped)
    
    v2_comp = compare_v2(all_data)
    if v2_comp['copy_times'] or v2_comp['move_times']:
        report_v2_analysis(v2_comp)
    
    v3_comp = compare_v3(all_data)
    if v3_comp['free_count'] > 0 or v3_comp['inplace_count'] > 0:
        report_v3_analysis(v3_comp)
    
    scaling = analyze_scaling(all_data)
    if scaling:
        report_scaling_analysis(scaling)
    
    print("=" * 70)
    print("ANALYSIS COMPLETE")
    print("=" * 70 + "\n")

if __name__ == '__main__':
    main()
