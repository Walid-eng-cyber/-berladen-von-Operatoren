#!/usr/bin/env python3

"""
Matrix Benchmark Analysis Script

Runs all matrix benchmarks and generates comprehensive CSV reports.
Supports filtering, aggregation, and comparison analysis.

Usage:
    python run_benchmarks.py                    # Run all benchmarks
    python run_benchmarks.py --speed fast       # Quick run
    python run_benchmarks.py --filter Addition  # Filter specific benchmarks
    python run_benchmarks.py --compare          # Compare original vs advanced
"""

import os
import sys
import subprocess
import csv
import json
from pathlib import Path
from datetime import datetime
from typing import List, Dict, Tuple
import statistics

# ============================================================
# Configuration
# ============================================================

PROJECT_DIR = r"c:\Users\walid\OneDrive\Desktop\-berladen-von-Operatoren"
BIN_DIR = os.path.join(PROJECT_DIR, "bin")
RESULTS_DIR = os.path.join(PROJECT_DIR, "results")
CODE_DIR = os.path.join(PROJECT_DIR, "code")

TIMESTAMP = datetime.now().strftime("%Y-%m-%d_%H%M%S")

BENCHMARK_TIME = {
    'fast': '50ms',
    'normal': '500ms',
    'thorough': '2s'
}

BENCHMARKS = {
    'original': {
        'exe': os.path.join(BIN_DIR, 'matrix_google_benchmark.exe'),
        'name': 'Original Suite (15 benchmarks)',
        'output': os.path.join(RESULTS_DIR, f'benchmark_original_{TIMESTAMP}.csv')
    },
    'advanced': {
        'exe': os.path.join(BIN_DIR, 'matrix_benchmark_advanced.exe'),
        'name': 'Advanced Suite (33 benchmarks)',
        'output': os.path.join(RESULTS_DIR, f'benchmark_advanced_{TIMESTAMP}.csv')
    }
}

# ============================================================
# Helper Functions
# ============================================================

def ensure_results_dir():
    """Create results directory if it doesn't exist"""
    Path(RESULTS_DIR).mkdir(parents=True, exist_ok=True)

def setup_environment():
    """Set up Windows environment with CMake and LLVM"""
    cmake_path = r"C:\cmake-portable\cmake-3.29.3-windows-x86_64\bin"
    llvm_path = r"C:\Users\walid\llvm-mingw-20260519-ucrt-x86_64\bin"
    
    current_path = os.environ.get('PATH', '')
    os.environ['PATH'] = f"{cmake_path};{llvm_path};{current_path}"

def run_benchmark(benchmark_name: str, exe_path: str, output_csv: str, 
                 time_setting: str = '500ms', filter_pattern: str = '') -> bool:
    """Run a single benchmark and export to CSV"""
    
    if not os.path.exists(exe_path):
        print(f"✗ Executable not found: {exe_path}")
        return False
    
    print(f"\nRunning: {benchmark_name}")
    print(f"  Time per benchmark: {time_setting}")
    
    # Build command
    cmd = [
        exe_path,
        f'--benchmark_out={output_csv}',
        '--benchmark_out_format=csv',
        f'--benchmark_min_time={time_setting}'
    ]
    
    if filter_pattern:
        cmd.append(f'--benchmark_filter={filter_pattern}')
    
    try:
        subprocess.run(cmd, capture_output=True, check=True, timeout=600)
        
        if os.path.exists(output_csv):
            file_size = os.path.getsize(output_csv) / 1024
            print(f"✓ Exported: {output_csv} ({file_size:.2f} KB)")
            return True
        else:
            print("✗ Output file not created")
            return False
            
    except subprocess.TimeoutExpired:
        print("✗ Benchmark timed out")
        return False
    except Exception as e:
        print(f"✗ Error: {str(e)}")
        return False

def parse_csv(filepath: str) -> List[Dict]:
    """Parse benchmark CSV file"""
    data = []
    
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            reader = csv.DictReader(f)
            data = list(reader)
    except Exception as e:
        print(f"Error reading {filepath}: {str(e)}")
    
    return data

def categorize_benchmark(name: str) -> str:
    """Categorize benchmark by name"""
    if 'Chained' in name:
        return 'Chained Operations'
    elif 'Multiply' in name:
        return 'Matrix Multiplication'
    elif 'InPlace' in name:
        return 'In-Place Operations'
    elif 'FreeFunc' in name:
        return 'Free vs In-Place'
    elif 'Iterative' in name:
        return 'Iterative Refinement'
    elif 'Scaling' in name:
        return 'Scaling Analysis'
    else:
        return 'Other'

def analyze_results(combined_data: List[Dict]) -> Dict:
    """Analyze benchmark results"""
    
    analysis = {
        'total_benchmarks': len(combined_data),
        'by_category': {},
        'v2_comparison': {},
        'v3_comparison': {},
        'scaling_data': []
    }
    
    # Group by category
    for row in combined_data:
        category = categorize_benchmark(row.get('name', ''))
        if category not in analysis['by_category']:
            analysis['by_category'][category] = []
        analysis['by_category'][category].append(row)
    
    # Find V2 comparison (Copy vs Move)
    copy_benches = [r for r in combined_data if 'Copy_Constructor' in r.get('name', '')]
    move_benches = [r for r in combined_data if 'Move_Constructor' in r.get('name', '')]
    
    if copy_benches and move_benches:
        analysis['v2_comparison'] = {
            'copy': copy_benches,
            'move': move_benches
        }
    
    # Find V3 comparison (Free vs In-Place)
    free_benches = [r for r in combined_data if 'FreeFunc' in r.get('name', '') and 'Free' in r.get('name', '')]
    inplace_benches = [r for r in combined_data if 'InPlace' in r.get('name', '')]
    
    if free_benches or inplace_benches:
        analysis['v3_comparison'] = {
            'free': free_benches,
            'inplace': inplace_benches
        }
    
    # Scaling data
    scaling = [r for r in combined_data if 'Scaling' in r.get('name', '')]
    analysis['scaling_data'] = scaling
    
    return analysis

def generate_analysis_report(analysis: Dict, output_file: str):
    """Generate text analysis report"""
    
    with open(output_file, 'w', encoding='utf-8') as f:
        f.write("=" * 70 + "\n")
        f.write("MATRIX BENCHMARK ANALYSIS REPORT\n")
        f.write("=" * 70 + "\n\n")
        f.write(f"Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}\n\n")
        
        # Summary
        f.write("SUMMARY\n")
        f.write("-" * 70 + "\n")
        f.write(f"Total Benchmarks: {analysis['total_benchmarks']}\n\n")
        
        # By Category
        f.write("BENCHMARKS BY CATEGORY\n")
        f.write("-" * 70 + "\n")
        for category, benches in analysis['by_category'].items():
            f.write(f"  {category}: {len(benches)} benchmarks\n")
        f.write("\n")
        
        # V2 Analysis
        if analysis['v2_comparison']:
            f.write("V2 PERFORMANCE (Move Semantics)\n")
            f.write("-" * 70 + "\n")
            
            copy_data = analysis['v2_comparison'].get('copy', [])
            move_data = analysis['v2_comparison'].get('move', [])
            
            if copy_data and move_data:
                f.write(f"Copy Constructor Benchmarks: {len(copy_data)}\n")
                for b in copy_data:
                    f.write(f"  {b.get('name', 'N/A')}: {b.get('time', 'N/A')}\n")
                
                f.write(f"\nMove Constructor Benchmarks: {len(move_data)}\n")
                for b in move_data:
                    f.write(f"  {b.get('name', 'N/A')}: {b.get('time', 'N/A')}\n")
            f.write("\n")
        
        # V3 Analysis
        if analysis['v3_comparison']:
            f.write("V3 PERFORMANCE (Compound Operators)\n")
            f.write("-" * 70 + "\n")
            
            free_data = analysis['v3_comparison'].get('free', [])
            inplace_data = analysis['v3_comparison'].get('inplace', [])
            
            if free_data:
                f.write(f"Free Function (+): {len(free_data)} benchmarks\n")
                for b in free_data:
                    f.write(f"  {b.get('name', 'N/A')}: {b.get('time', 'N/A')}\n")
                f.write("\n")
            
            if inplace_data:
                f.write(f"In-Place Operators (+=): {len(inplace_data)} benchmarks\n")
                for b in inplace_data:
                    f.write(f"  {b.get('name', 'N/A')}: {b.get('time', 'N/A')}\n")
            f.write("\n")
        
        # Scaling Analysis
        if analysis['scaling_data']:
            f.write("SCALING ANALYSIS\n")
            f.write("-" * 70 + "\n")
            f.write(f"Total scaling benchmarks: {len(analysis['scaling_data'])}\n\n")
            
            f.write("Matrix Size Progression:\n")
            for b in analysis['scaling_data']:
                name = b.get('name', 'N/A')
                time = b.get('time', 'N/A')
                f.write(f"  {name}: {time}\n")
            f.write("\n")
        
        # Footer
        f.write("=" * 70 + "\n")
        f.write("END OF REPORT\n")
        f.write("=" * 70 + "\n")

def main():
    """Main execution"""
    
    print("\n" + "=" * 70)
    print("MATRIX BENCHMARK CSV EXPORT TOOL")
    print("=" * 70)
    
    # Setup
    setup_environment()
    ensure_results_dir()
    
    speed = 'normal'  # Default
    if len(sys.argv) > 1 and '--speed' in sys.argv:
        idx = sys.argv.index('--speed')
        if idx + 1 < len(sys.argv):
            speed = sys.argv[idx + 1]
    
    time_setting = BENCHMARK_TIME.get(speed, BENCHMARK_TIME['normal'])
    
    print(f"\nSettings:")
    print(f"  Mode: {speed}")
    print(f"  Time per benchmark: {time_setting}")
    print(f"  Results directory: {RESULTS_DIR}\n")
    
    # Run benchmarks
    results_files = []
    all_data = []
    
    for bench_key, bench_info in BENCHMARKS.items():
        success = run_benchmark(
            bench_info['name'],
            bench_info['exe'],
            bench_info['output'],
            time_setting
        )
        
        if success:
            results_files.append(bench_info['output'])
            data = parse_csv(bench_info['output'])
            all_data.extend(data)
    
    # Combine and analyze
    if all_data:
        combined_file = os.path.join(RESULTS_DIR, f'benchmark_combined_{TIMESTAMP}.csv')
        
        # Write combined CSV
        if all_data:
            fieldnames = all_data[0].keys() if all_data else []
            with open(combined_file, 'w', newline='', encoding='utf-8') as f:
                writer = csv.DictWriter(f, fieldnames=fieldnames)
                writer.writeheader()
                writer.writerows(all_data)
        
        print(f"\n✓ Combined results: {combined_file}")
        
        # Generate analysis
        analysis = analyze_results(all_data)
        analysis_file = os.path.join(RESULTS_DIR, f'analysis_{TIMESTAMP}.txt')
        generate_analysis_report(analysis, analysis_file)
        
        print(f"✓ Analysis report: {analysis_file}")
        
        # Summary
        print("\n" + "=" * 70)
        print("BENCHMARK EXPORT COMPLETE")
        print("=" * 70)
        print(f"\nTotal Benchmarks: {len(all_data)}")
        print(f"Benchmarks by Category:")
        for category, benches in analysis['by_category'].items():
            print(f"  {category}: {len(benches)}")
        
        print(f"\nOutput Files:")
        print(f"  Combined CSV: {combined_file}")
        print(f"  Analysis: {analysis_file}")
        
        if len(results_files) > 1:
            for f in results_files:
                print(f"  Individual: {f}")
        
        print("\nNext Steps:")
        print("  1. Open CSV in Excel for charts and analysis")
        print("  2. Use Python pandas for further processing")
        print("  3. Compare V2 vs V3 performance from analysis report")
        
    else:
        print("\n✗ No benchmark results generated")
        sys.exit(1)

if __name__ == '__main__':
    main()
