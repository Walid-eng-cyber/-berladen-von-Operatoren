#!/usr/bin/env python3
"""
Analyze benchmark results across different optimization levels (-O0, -O2, -O3).
Compares performance metrics and calculates speedups.
"""

import csv
import sys
import os
import re
import statistics
from pathlib import Path
from collections import defaultdict

def parse_time_to_nanoseconds(time_str):
    """Convert time string (e.g., '1.5ms', '500us', '1000ns') to nanoseconds."""
    if isinstance(time_str, (int, float)):
        return float(time_str)
    
    time_str = str(time_str).strip()
    
    # Extract number and unit
    match = re.match(r'([\d.]+)\s*([a-zA-Z]+)', time_str)
    if not match:
        return float(time_str)
    
    value, unit = float(match.group(1)), match.group(2).lower()
    
    conversions = {
        'ns': 1,
        'us': 1000,
        'µs': 1000,
        'ms': 1_000_000,
        's': 1_000_000_000,
    }
    
    return value * conversions.get(unit, 1)

def load_csv_results(filepath):
    """Load benchmark results from CSV file."""
    results = defaultdict(lambda: {'times': [], 'cpu_time': None, 'wall_time': None})
    
    try:
        with open(filepath, 'r') as f:
            reader = csv.DictReader(f)
            for row in reader:
                if row.get('name') and 'time' in row.get('name', '').lower():
                    benchmark_name = row['name']
                    # Filter out aggregate lines
                    if 'Benchmark' in row.get('name', ''):
                        continue
                    
                    cpu_time = row.get('cpu_time', '0')
                    wall_time = row.get('wall_time', cpu_time)
                    
                    try:
                        cpu_ns = parse_time_to_nanoseconds(cpu_time)
                        results[benchmark_name]['times'].append(cpu_ns)
                        results[benchmark_name]['cpu_time'] = cpu_ns
                        results[benchmark_name]['wall_time'] = parse_time_to_nanoseconds(wall_time)
                    except (ValueError, TypeError):
                        pass
    except Exception as e:
        print(f"Warning: Could not read {filepath}: {e}", file=sys.stderr)
    
    return results

def extract_optimization_level(filepath):
    """Extract optimization level from filename (e.g., 'benchmark_original_O2_*.csv' → 'O2')."""
    match = re.search(r'_O(\d)_', filepath)
    return f"O{match.group(1)}" if match else "Unknown"

def format_time(nanoseconds):
    """Format nanoseconds to human-readable time."""
    if nanoseconds < 1000:
        return f"{nanoseconds:.1f} ns"
    elif nanoseconds < 1_000_000:
        return f"{nanoseconds / 1000:.2f} µs"
    elif nanoseconds < 1_000_000_000:
        return f"{nanoseconds / 1_000_000:.3f} ms"
    else:
        return f"{nanoseconds / 1_000_000_000:.3f} s"

def main():
    if len(sys.argv) < 2:
        print("Usage: python analyze_optimization_levels.py <csv_file_pattern> [<csv_file2> ...]")
        print("Example: python analyze_optimization_levels.py results/benchmark_*_O*.csv")
        sys.exit(1)
    
    # Collect all CSV files
    csv_files = []
    for pattern in sys.argv[1:]:
        if '*' in pattern or '?' in pattern:
            from glob import glob
            csv_files.extend(glob(pattern))
        else:
            csv_files.append(pattern)
    
    if not csv_files:
        print("No CSV files found matching the pattern(s).")
        sys.exit(1)
    
    csv_files = sorted(set(csv_files))
    
    print("╔════════════════════════════════════════════════════════════╗")
    print("║   Optimization Level Performance Analysis                  ║")
    print("╚════════════════════════════════════════════════════════════╝")
    print()
    
    # Group results by optimization level
    results_by_opt = {}
    for filepath in csv_files:
        opt_level = extract_optimization_level(filepath)
        if opt_level not in results_by_opt:
            results_by_opt[opt_level] = {}
        
        results = load_csv_results(filepath)
        results_by_opt[opt_level].update(results)
    
    # Sort optimization levels
    opt_order = ['O0', 'O2', 'O3']
    sorted_opts = sorted(results_by_opt.keys(), key=lambda x: (opt_order.index(x) if x in opt_order else 999, x))
    
    print(f"Optimization levels found: {', '.join(sorted_opts)}\n")
    
    # Analyze each optimization level
    print("═" * 62)
    print("Individual Optimization Level Analysis")
    print("═" * 62)
    
    stats_by_opt = {}
    for opt_level in sorted_opts:
        results = results_by_opt[opt_level]
        times = [v['times'][0] for v in results.values() if v['times']]
        
        if times:
            stats = {
                'min': min(times),
                'max': max(times),
                'mean': statistics.mean(times),
                'median': statistics.median(times),
                'stdev': statistics.stdev(times) if len(times) > 1 else 0,
            }
            stats_by_opt[opt_level] = stats
            
            print(f"\n{opt_level}:")
            print(f"  Benchmarks: {len(times)}")
            print(f"  Min:        {format_time(stats['min'])}")
            print(f"  Max:        {format_time(stats['max'])}")
            print(f"  Mean:       {format_time(stats['mean'])}")
            print(f"  Median:     {format_time(stats['median'])}")
            print(f"  Stdev:      {format_time(stats['stdev'])}")
    
    # Compare optimization levels
    print("\n" + "═" * 62)
    print("Optimization Level Comparison")
    print("═" * 62)
    
    if len(sorted_opts) >= 2:
        baseline_opt = sorted_opts[0]
        baseline_stats = stats_by_opt[baseline_opt]
        
        print(f"\nBaseline: {baseline_opt}")
        print(f"  Mean time: {format_time(baseline_stats['mean'])}")
        print()
        
        for opt_level in sorted_opts[1:]:
            current_stats = stats_by_opt[opt_level]
            speedup = baseline_stats['mean'] / current_stats['mean']
            percent_faster = (speedup - 1) * 100
            
            print(f"{opt_level} vs {baseline_opt}:")
            print(f"  {opt_level} mean: {format_time(current_stats['mean'])}")
            if speedup > 1:
                print(f"  ✓ {speedup:.2f}× faster ({percent_faster:+.1f}%)")
            else:
                print(f"  ✗ {abs(speedup - 1):.2f}× slower ({percent_faster:+.1f}%)")
            print()
    
    # Detailed comparison by benchmark category
    print("═" * 62)
    print("Performance Breakdown by Benchmark")
    print("═" * 62)
    
    # Extract common benchmark names across all optimization levels
    all_benchmarks = set()
    for results in results_by_opt.values():
        all_benchmarks.update(results.keys())
    
    if all_benchmarks and len(sorted_opts) > 1:
        print()
        print(f"{'Benchmark':<40} {' | '.join(f'{opt:>8}' for opt in sorted_opts)} | {'Speedup':>8}")
        print("─" * 100)
        
        for bench_name in sorted(all_benchmarks)[:20]:  # Show top 20
            times = []
            for opt_level in sorted_opts:
                results = results_by_opt[opt_level]
                if bench_name in results and results[bench_name]['times']:
                    times.append(results[bench_name]['times'][0])
                else:
                    times.append(None)
            
            # Only show if we have data for at least 2 levels
            if sum(1 for t in times if t is not None) >= 2:
                time_strs = []
                for t in times:
                    if t is not None:
                        if t < 1000:
                            time_strs.append(f"{t:>6.0f}ns")
                        elif t < 1_000_000:
                            time_strs.append(f"{t/1000:>6.1f}µs")
                        else:
                            time_strs.append(f"{t/1_000_000:>6.2f}ms")
                    else:
                        time_strs.append("    -  ")
                
                speedup_str = ""
                if times[0] and times[-1]:
                    speedup = times[0] / times[-1]
                    speedup_str = f"{speedup:>7.2f}×"
                
                short_name = bench_name[:38] if len(bench_name) <= 38 else bench_name[:35] + "..."
                print(f"{short_name:<40} {' | '.join(time_strs)} | {speedup_str:>8}")
    
    print("\n" + "═" * 62)
    print("Summary")
    print("═" * 62)
    print()
    print("Recommendations:")
    print("  • O0: Use for debugging (no optimizations)")
    print("  • O2: Production standard (optimized, still debuggable)")
    print("  • O3: Maximum performance (may impact compile time)")
    print()
    
    # Find best optimization level for median performance
    if stats_by_opt:
        best_opt = min(stats_by_opt.keys(), key=lambda x: stats_by_opt[x]['median'])
        print(f"✓ Best median performance: {best_opt} ({format_time(stats_by_opt[best_opt]['median'])})")
    
    print()

if __name__ == "__main__":
    main()
