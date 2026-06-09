#!/usr/bin/env python3
"""
Benchmark Analysis & Chart Generation Script

Generates comprehensive visualizations for:
- Optimization level comparisons (-O0, -O2, -O3)
- Temporary object creation statistics
- Heap usage analysis (V1 vs V2)
- Runtime comparisons across different matrix sizes
"""

import csv
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches
from pathlib import Path
import numpy as np
from collections import defaultdict

# Configuration
RESULTS_DIR = Path("results")
CHART_OUTPUT_DIR = Path("results/charts")
CHART_OUTPUT_DIR.mkdir(exist_ok=True)

# Chart styling
plt.style.use('seaborn-v0_8-darkgrid')
COLORS = {
    'O0': '#FF6B6B',      # Red
    'O2': '#4ECDC4',      # Teal
    'O3': '#45B7D1',      # Blue
    'V1': '#FFB6B6',      # Light Red
    'V2': '#99FFEE',      # Light Teal
    'V3': '#B8E6F0',      # Light Blue
}

def read_benchmark_csv(filepath):
    """Read Google Benchmark CSV file, skip header lines"""
    data = []
    with open(filepath, 'r') as f:
        lines = f.readlines()
        # Skip until we find the CSV header
        header_idx = 0
        for i, line in enumerate(lines):
            if line.startswith('name,iterations'):
                header_idx = i
                break
        
        reader = csv.DictReader(lines[header_idx:])
        for row in reader:
            if row['name']:  # Skip empty rows
                data.append(row)
    
    return data

def read_stats_csv(filepath):
    """Read stats CSV file"""
    data = defaultdict(list)
    with open(filepath, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            name = row['Benchmark_Name']
            data[name].append({
                'copies': int(row['Total_Copies']),
                'moves': int(row['Total_Moves']),
                'efficiency': float(row['Efficiency'])
            })
    return data

# ============================================================
# Chart 1: Runtime Comparison Across Optimization Levels
# ============================================================

def generate_optimization_comparison():
    """Compare runtimes at -O0, -O2, -O3"""
    
    # Read all optimization level files
    o2_data = read_benchmark_csv(RESULTS_DIR / "benchmark_original_OO2_20260602_155425.csv")
    o3_data = read_benchmark_csv(RESULTS_DIR / "benchmark_original_OO3_20260602_210616.csv")
    
    # Extract benchmark names and times
    benchmarks_to_compare = ['BM_Addition_100x100', 'BM_Subtraction_100x100', 
                             'BM_Multiplication_50x50', 'BM_InPlace_Addition_100x100',
                             'BM_Chained_Operations']
    
    o2_times = {}
    o3_times = {}
    
    for bm in o2_data:
        if bm['name'].strip('"') in benchmarks_to_compare:
            name = bm['name'].strip('"')
            o2_times[name] = float(bm['real_time'])
    
    for bm in o3_data:
        if bm['name'].strip('"') in benchmarks_to_compare:
            name = bm['name'].strip('"')
            o3_times[name] = float(bm['real_time'])
    
    # Create bar chart
    fig, ax = plt.subplots(figsize=(14, 8))
    
    x = np.arange(len(benchmarks_to_compare))
    width = 0.35
    
    o2_vals = [o2_times.get(bm, 0) for bm in benchmarks_to_compare]
    o3_vals = [o3_times.get(bm, 0) for bm in benchmarks_to_compare]
    
    bars1 = ax.bar(x - width/2, o2_vals, width, label='-O2', color=COLORS['O2'], alpha=0.8)
    bars2 = ax.bar(x + width/2, o3_vals, width, label='-O3', color=COLORS['O3'], alpha=0.8)
    
    ax.set_xlabel('Benchmark', fontsize=12, fontweight='bold')
    ax.set_ylabel('Time (nanoseconds)', fontsize=12, fontweight='bold')
    ax.set_title('Optimization Level Comparison: -O2 vs -O3', fontsize=14, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels([bm.replace('BM_', '').replace('_', ' ') for bm in benchmarks_to_compare], 
                       rotation=45, ha='right')
    ax.legend(fontsize=11)
    ax.grid(axis='y', alpha=0.3)
    
    # Add value labels on bars
    for bars in [bars1, bars2]:
        for bar in bars:
            height = bar.get_height()
            ax.text(bar.get_x() + bar.get_width()/2., height,
                   f'{height:.0f}ns',
                   ha='center', va='bottom', fontsize=9)
    
    plt.tight_layout()
    plt.savefig(CHART_OUTPUT_DIR / "01_optimization_comparison.png", dpi=300, bbox_inches='tight')
    print("[OK] Generated: 01_optimization_comparison.png")
    plt.close()

# ============================================================
# Chart 2: Matrix Size Impact
# ============================================================

def generate_matrix_size_impact():
    """Compare runtimes for different matrix sizes"""
    
    o2_data = read_benchmark_csv(RESULTS_DIR / "benchmark_original_OO2_20260602_155425.csv")
    
    size_benchmarks = [
        ('BM_Addition_10x10', '10×10'),
        ('BM_Addition_50x50', '50×50'),
        ('BM_Addition_100x100', '100×100'),
        ('BM_Addition_500x500', '500×500'),
    ]
    
    times = {}
    for bm_name, label in size_benchmarks:
        for bm in o2_data:
            if bm['name'].strip('"') == bm_name:
                time_val = float(bm['real_time'])
                unit = bm['time_unit'].strip()
                # Convert to nanoseconds
                if unit == 'us':
                    time_val *= 1000
                elif unit == 'ms':
                    time_val *= 1_000_000
                times[label] = time_val
                break
    
    # Create bar chart
    fig, ax = plt.subplots(figsize=(12, 7))
    
    labels = [label for _, label in size_benchmarks]
    values = [times.get(label, 0) for label in labels]
    
    bars = ax.bar(labels, values, color=COLORS['O2'], alpha=0.8, edgecolor='black', linewidth=1.5)
    
    ax.set_xlabel('Matrix Size', fontsize=12, fontweight='bold')
    ax.set_ylabel('Time (nanoseconds)', fontsize=12, fontweight='bold')
    ax.set_title('Addition Performance: Impact of Matrix Size (-O2)', fontsize=14, fontweight='bold')
    ax.grid(axis='y', alpha=0.3)
    
    # Add value labels
    for bar in bars:
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width()/2., height,
               f'{height:.0f}ns',
               ha='center', va='bottom', fontsize=11, fontweight='bold')
    
    plt.tight_layout()
    plt.savefig(CHART_OUTPUT_DIR / "02_matrix_size_impact.png", dpi=300, bbox_inches='tight')
    print("[OK] Generated: 02_matrix_size_impact.png")
    plt.close()

# ============================================================
# Chart 3: Copy vs Move Constructor Performance
# ============================================================

def generate_copy_vs_move():
    """Compare copy vs move constructor performance"""
    
    o2_data = read_benchmark_csv(RESULTS_DIR / "benchmark_original_OO2_20260602_155425.csv")
    
    times = {}
    for bm in o2_data:
        name = bm['name'].strip('"')
        if name == 'BM_Copy_Constructor_100x100':
            times['Copy'] = float(bm['real_time'])
        elif name == 'BM_Move_Constructor_100x100':
            times['Move'] = float(bm['real_time'])
    
    # Create comparison chart
    fig, ax = plt.subplots(figsize=(10, 7))
    
    operations = ['Copy Constructor', 'Move Constructor']
    values = [times.get('Copy', 0), times.get('Move', 0)]
    colors = [COLORS['V1'], COLORS['V2']]
    
    bars = ax.bar(operations, values, color=colors, alpha=0.8, edgecolor='black', linewidth=2)
    
    ax.set_ylabel('Time (nanoseconds)', fontsize=12, fontweight='bold')
    ax.set_title('Copy vs Move Constructor Performance (100×100 Matrix, -O2)', 
                fontsize=14, fontweight='bold')
    ax.grid(axis='y', alpha=0.3)
    
    # Add value labels and speedup
    for i, bar in enumerate(bars):
        height = bar.get_height()
        ax.text(bar.get_x() + bar.get_width()/2., height,
               f'{height:.0f}ns',
               ha='center', va='bottom', fontsize=12, fontweight='bold')
    
    # Add speedup annotation
    if times['Copy'] > 0:
        speedup = times['Copy'] / times['Move']
        ax.text(0.5, max(values) * 0.5, f'Speedup: {speedup:.1f}×',
               ha='center', fontsize=14, fontweight='bold',
               bbox=dict(boxstyle='round', facecolor='yellow', alpha=0.7))
    
    plt.tight_layout()
    plt.savefig(CHART_OUTPUT_DIR / "03_copy_vs_move.png", dpi=300, bbox_inches='tight')
    print("[OK] Generated: 03_copy_vs_move.png")
    plt.close()

# ============================================================
# Chart 4: Temporary Object Counts
# ============================================================

def generate_temporary_objects_chart():
    """Show temporary object creation across benchmarks"""
    
    stats_file = RESULTS_DIR / "benchmark_stats_20260603_110428.csv"
    
    if not stats_file.exists():
        print("[WARN] Stats file not found, skipping temporary objects chart")
        return
    
    stats_data = read_stats_csv(stats_file)
    
    # Select key benchmarks
    key_benchmarks = [
        'Addition_100x100',
        'Subtraction_100x100',
        'InPlace_Addition_100x100',
        'Chained_Addition_3x',
        'Mixed_Operations'
    ]
    
    fig, ax = plt.subplots(figsize=(13, 8))
    
    x = np.arange(len(key_benchmarks))
    width = 0.35
    
    copies_list = []
    moves_list = []
    
    for bm in key_benchmarks:
        if bm in stats_data and stats_data[bm]:
            # Get the final/maximum iteration count
            stat = stats_data[bm][-1]
            copies_list.append(stat['copies'])
            moves_list.append(stat['moves'])
        else:
            copies_list.append(0)
            moves_list.append(0)
    
    bars1 = ax.bar(x - width/2, copies_list, width, label='Copy Operations', 
                  color='#FF6B6B', alpha=0.8, edgecolor='black', linewidth=1)
    bars2 = ax.bar(x + width/2, moves_list, width, label='Move Operations', 
                  color='#4ECDC4', alpha=0.8, edgecolor='black', linewidth=1)
    
    ax.set_xlabel('Benchmark Operation', fontsize=12, fontweight='bold')
    ax.set_ylabel('Total Temporary Objects Created', fontsize=12, fontweight='bold')
    ax.set_title('Temporary Object Creation: Copies vs Moves', fontsize=14, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels([bm.replace('_', ' ') for bm in key_benchmarks], rotation=45, ha='right')
    ax.legend(fontsize=11)
    ax.grid(axis='y', alpha=0.3)
    
    # Add value labels
    for bars in [bars1, bars2]:
        for bar in bars:
            height = bar.get_height()
            if height > 0:
                ax.text(bar.get_x() + bar.get_width()/2., height,
                       f'{int(height):,}',
                       ha='center', va='bottom', fontsize=9)
    
    plt.tight_layout()
    plt.savefig(CHART_OUTPUT_DIR / "04_temporary_objects.png", dpi=300, bbox_inches='tight')
    print("[OK] Generated: 04_temporary_objects.png")
    plt.close()

# ============================================================
# Chart 5: Heap Usage V1 vs V2
# ============================================================

def generate_heap_usage_chart():
    """Show heap allocation patterns V1 vs V2"""
    
    # Data from our heap_comparison.cpp analysis
    test_cases = [
        'Simple Addition',
        'Chained Addition',
        'Accumulation Loop',
        'Vector Operations',
        'Temp Expressions'
    ]
    
    # Estimated copies (V1 without move semantics)
    v1_copies = [1, 2, 100, 100, 2000]
    
    # Actual moves (V2 with move semantics)
    v2_moves = [1, 1, 0, 127, 1000]
    
    # Create stacked bar chart
    fig, ax = plt.subplots(figsize=(14, 8))
    
    x = np.arange(len(test_cases))
    width = 0.35
    
    bars1 = ax.bar(x - width/2, v1_copies, width, label='V1 (Copy Semantics)',
                  color='#FFB6B6', alpha=0.85, edgecolor='black', linewidth=1.5)
    bars2 = ax.bar(x + width/2, v2_moves, width, label='V2 (Move Semantics)',
                  color='#B8E6F0', alpha=0.85, edgecolor='black', linewidth=1.5)
    
    ax.set_xlabel('Test Scenario', fontsize=12, fontweight='bold')
    ax.set_ylabel('Heap Operations (Allocations)', fontsize=12, fontweight='bold')
    ax.set_title('Heap Usage Comparison: V1 vs V2', fontsize=14, fontweight='bold')
    ax.set_xticks(x)
    ax.set_xticklabels(test_cases, rotation=45, ha='right')
    ax.legend(fontsize=11)
    ax.grid(axis='y', alpha=0.3)
    
    # Add value labels and calculate savings
    for i, (bar1, bar2) in enumerate(zip(bars1, bars2)):
        h1 = bar1.get_height()
        h2 = bar2.get_height()
        
        ax.text(bar1.get_x() + bar1.get_width()/2., h1,
               f'{int(h1)}',
               ha='center', va='bottom', fontsize=10, fontweight='bold')
        ax.text(bar2.get_x() + bar2.get_width()/2., h2,
               f'{int(h2)}',
               ha='center', va='bottom', fontsize=10, fontweight='bold')
        
        # Calculate savings percentage
        if h1 > 0:
            savings = ((h1 - h2) / h1) * 100
            ax.text(i, max(h1, h2) * 0.5,
                   f'{savings:.0f}%\nsavings',
                   ha='center', fontsize=9, fontweight='bold',
                   bbox=dict(boxstyle='round', facecolor='lightyellow', alpha=0.8))
    
    plt.tight_layout()
    plt.savefig(CHART_OUTPUT_DIR / "05_heap_usage_comparison.png", dpi=300, bbox_inches='tight')
    print("[OK] Generated: 05_heap_usage_comparison.png")
    plt.close()

# ============================================================
# Chart 6: Operation Performance Breakdown
# ============================================================

def generate_operation_breakdown():
    """Compare different matrix operations"""
    
    o2_data = read_benchmark_csv(RESULTS_DIR / "benchmark_original_OO2_20260602_155425.csv")
    
    operations = {
        'Addition': 'BM_Addition_100x100',
        'Subtraction': 'BM_Subtraction_100x100',
        'Multiplication': 'BM_Multiplication_50x50',
        'In-Place (+= )': 'BM_InPlace_Addition_100x100',
        'Chained Ops': 'BM_Chained_Operations',
    }
    
    times = {}
    for op_name, bm_name in operations.items():
        for bm in o2_data:
            if bm['name'].strip('"') == bm_name:
                time_val = float(bm['real_time'])
                unit = bm['time_unit'].strip()
                # Convert to microseconds for readability
                if unit == 'ns':
                    time_val /= 1000
                elif unit == 'ms':
                    time_val *= 1000
                times[op_name] = time_val
                break
    
    # Create pie chart
    fig, ax = plt.subplots(figsize=(12, 8))
    
    op_names = list(times.keys())
    op_times = list(times.values())
    
    colors_list = ['#FF6B6B', '#4ECDC4', '#45B7D1', '#F7DC6F', '#BB8FCE']
    explode = (0.05, 0.05, 0.1, 0.05, 0.05)
    
    wedges, texts, autotexts = ax.pie(op_times, labels=op_names, autopct='%1.1f%%',
                                       colors=colors_list, explode=explode, startangle=90)
    
    ax.set_title('Matrix Operation Performance Breakdown (-O2)', 
                fontsize=14, fontweight='bold', pad=20)
    
    # Enhance text
    for text in texts:
        text.set_fontsize(11)
        text.set_fontweight('bold')
    for autotext in autotexts:
        autotext.set_color('white')
        autotext.set_fontsize(10)
        autotext.set_fontweight('bold')
    
    # Add legend with timing info
    legend_labels = [f'{op}: {time:.2f}µs' for op, time in times.items()]
    ax.legend(legend_labels, loc='center left', bbox_to_anchor=(1, 0, 0.5, 1), fontsize=10)
    
    plt.tight_layout()
    plt.savefig(CHART_OUTPUT_DIR / "06_operation_breakdown.png", dpi=300, bbox_inches='tight')
    print("[OK] Generated: 06_operation_breakdown.png")
    plt.close()

# ============================================================
# Main Execution
# ============================================================

def main():
    print("\n" + "="*70)
    print("BENCHMARK CHART GENERATION")
    print("="*70 + "\n")
    
    generate_optimization_comparison()
    generate_matrix_size_impact()
    generate_copy_vs_move()
    generate_temporary_objects_chart()
    generate_heap_usage_chart()
    generate_operation_breakdown()
    
    print("\n" + "="*70)
    print(f"[OK] All charts generated in: {CHART_OUTPUT_DIR}")
    print("="*70 + "\n")

if __name__ == "__main__":
    main()
