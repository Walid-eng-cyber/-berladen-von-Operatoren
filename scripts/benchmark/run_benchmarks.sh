#!/bin/bash

# Matrix Benchmark CSV Export Script (Bash Version for Linux/macOS)
#
# Runs all matrix benchmarks and exports results to CSV format.
#
# Usage:
#   chmod +x run_benchmarks.sh
#   ./run_benchmarks.sh [fast|normal|thorough]
#
# Environment (Linux/macOS with GCC or Clang):
#   Ensure g++ or clang++ is in PATH

set -e

# ============================================================
# Configuration
# ============================================================

PROJECT_DIR="${PROJECT_DIR:-.}"
BIN_DIR="$PROJECT_DIR/bin"
RESULTS_DIR="$PROJECT_DIR/results"
TIMESTAMP=$(date +"%Y-%m-%d_%H%M%S")

# Benchmark time settings
SPEED="${1:-normal}"
case "$SPEED" in
    fast)
        BENCHMARK_TIME="50ms"
        ;;
    normal)
        BENCHMARK_TIME="500ms"
        ;;
    thorough)
        BENCHMARK_TIME="2s"
        ;;
    *)
        echo "Usage: $0 [fast|normal|thorough]"
        exit 1
        ;;
esac

# ============================================================
# Colors
# ============================================================

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# ============================================================
# Functions
# ============================================================

print_header() {
    echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${CYAN}║       Matrix Benchmarks - CSV Export (Linux/macOS)         ║${NC}"
    echo -e "${CYAN}║       Mode: $SPEED (Time: $BENCHMARK_TIME)${NC}"
    echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
    echo ""
}

ensure_dirs() {
    mkdir -p "$RESULTS_DIR"
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓${NC} Created results directory: $RESULTS_DIR"
    fi
}

run_benchmark() {
    local name="$1"
    local exe="$2"
    local output="$3"
    local filter="${4:-}"
    
    echo ""
    echo -e "${YELLOW}Running:${NC} $name"
    echo -e "${YELLOW}Time per benchmark:${NC} $BENCHMARK_TIME"
    
    if [ ! -f "$exe" ]; then
        echo -e "${RED}✗ Executable not found:${NC} $exe"
        return 1
    fi
    
    # Run benchmark
    if [ -z "$filter" ]; then
        "$exe" --benchmark_out="$output" --benchmark_out_format=csv --benchmark_min_time="$BENCHMARK_TIME" > /dev/null 2>&1
    else
        "$exe" --benchmark_filter="$filter" --benchmark_out="$output" --benchmark_out_format=csv --benchmark_min_time="$BENCHMARK_TIME" > /dev/null 2>&1
    fi
    
    if [ -f "$output" ]; then
        local size=$(du -k "$output" | cut -f1)
        echo -e "${GREEN}✓ Exported:${NC} $output ($size KB)"
        return 0
    else
        echo -e "${RED}✗ Export failed${NC}"
        return 1
    fi
}

combine_csv() {
    local output="$1"
    shift
    local files=("$@")
    
    if [ ${#files[@]} -eq 0 ]; then
        return 1
    fi
    
    # Combine CSVs (keep header from first file)
    {
        head -1 "${files[0]}"
        for file in "${files[@]}"; do
            tail -n +2 "$file"
        done
    } > "$output"
    
    return 0
}

# ============================================================
# Main
# ============================================================

print_header

ensure_dirs

cd "$PROJECT_DIR"

# Arrays to store results
declare -a RESULT_FILES=()
COMBINED_OUTPUT="$RESULTS_DIR/benchmark_combined_$TIMESTAMP.csv"
ANALYSIS_OUTPUT="$RESULTS_DIR/analysis_$TIMESTAMP.txt"

echo -e "${YELLOW}Starting benchmark runs...${NC}"

# Run original suite
ORIGINAL_OUTPUT="$RESULTS_DIR/benchmark_original_$TIMESTAMP.csv"
if run_benchmark "Original Suite (15 benchmarks)" \
    "$BIN_DIR/matrix_google_benchmark" \
    "$ORIGINAL_OUTPUT"; then
    RESULT_FILES+=("$ORIGINAL_OUTPUT")
fi

# Run advanced suite (if available)
ADVANCED_OUTPUT="$RESULTS_DIR/benchmark_advanced_$TIMESTAMP.csv"
if [ -f "$BIN_DIR/matrix_benchmark_advanced" ]; then
    if run_benchmark "Advanced Suite (33 benchmarks)" \
        "$BIN_DIR/matrix_benchmark_advanced" \
        "$ADVANCED_OUTPUT"; then
        RESULT_FILES+=("$ADVANCED_OUTPUT")
    fi
fi

# ============================================================
# Combine Results
# ============================================================

echo ""
echo -e "${YELLOW}Processing results...${NC}"

if combine_csv "$COMBINED_OUTPUT" "${RESULT_FILES[@]}"; then
    local combined_size=$(du -k "$COMBINED_OUTPUT" | cut -f1)
    echo -e "${GREEN}✓ Combined results:${NC} $COMBINED_OUTPUT ($combined_size KB)"
else
    echo -e "${RED}✗ Failed to combine results${NC}"
fi

# ============================================================
# Generate Analysis
# ============================================================

{
    echo "Matrix Benchmark Analysis Report"
    echo "Generated: $(date '+%Y-%m-%d %H:%M:%S')"
    echo "========================================"
    echo ""
    echo "Summary:"
    echo "  Mode: $SPEED"
    echo "  Benchmark Time: $BENCHMARK_TIME"
    echo "  Result Files: ${#RESULT_FILES[@]}"
    echo ""
    
    if [ -f "$COMBINED_OUTPUT" ]; then
        local total_lines=$(tail -1 "$COMBINED_OUTPUT" | wc -l)
        local total_benches=$(tail -n +2 "$COMBINED_OUTPUT" | wc -l)
        echo "  Total Benchmarks: $total_benches"
        echo ""
    fi
    
    echo "Output Files:"
    for file in "${RESULT_FILES[@]}"; do
        echo "  - $(basename $file)"
    done
    
    if [ -f "$COMBINED_OUTPUT" ]; then
        echo "  - $(basename $COMBINED_OUTPUT) (combined)"
    fi
    
} > "$ANALYSIS_OUTPUT"

# ============================================================
# Summary
# ============================================================

echo ""
echo -e "${GREEN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║              Benchmark Export Complete! ✓                  ║${NC}"
echo -e "${GREEN}╚════════════════════════════════════════════════════════════╝${NC}"

echo ""
echo -e "${CYAN}Output Files:${NC}"

if [ -f "$COMBINED_OUTPUT" ]; then
    echo -e "  ${GREEN}Combined:${NC}  $COMBINED_OUTPUT"
fi

for file in "${RESULT_FILES[@]}"; do
    if [ -f "$file" ]; then
        echo -e "  ${GREEN}Result:${NC}    $file"
    fi
done

echo -e "  ${GREEN}Analysis:${NC}   $ANALYSIS_OUTPUT"

echo ""
echo -e "${CYAN}Next Steps:${NC}"
echo "  1. View analysis: cat $ANALYSIS_OUTPUT"
echo "  2. Import to Excel/Calc for charts"
echo "  3. Process with Python for advanced analysis"
echo ""
echo -e "${GREEN}✓ Done!${NC}"
