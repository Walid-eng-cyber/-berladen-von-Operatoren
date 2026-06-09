#!/bin/bash
#
# Benchmark across different optimization levels (-O0, -O2, -O3)
# Cross-platform version for Linux and macOS
#

set -e

# Configuration
CLANG_PATH="${CLANG_PATH:-clang++}"
CMAKE_PATH="${CMAKE_PATH:-cmake}"
BENCHMARK_DIR="${BENCHMARK_DIR:-/usr/local/benchmark}"
SOURCE_DIR="code"
BIN_DIR="bin"
OBJ_DIR="obj"
RESULTS_DIR="results"
CXX_STANDARD="c++17"

# Optimization levels to test
OPTIMIZATION_LEVELS=("-O0" "-O2" "-O3")

# Benchmark min time based on speed setting
SPEED="${1:-normal}"
case $SPEED in
    fast)
        MIN_TIME="50ms"
        ;;
    normal)
        MIN_TIME="500ms"
        ;;
    thorough)
        MIN_TIME="2000ms"
        ;;
    *)
        echo "Usage: $0 [fast|normal|thorough]"
        exit 1
        ;;
esac

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Header
echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║   Multi-Optimization Benchmark Suite (-O0, -O2, -O3)      ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${YELLOW}Speed Setting: $SPEED (min time: $MIN_TIME)${NC}"
echo -e "${YELLOW}Optimization Levels: ${OPTIMIZATION_LEVELS[@]}${NC}"
echo ""

# Verify clang++ exists
if ! command -v "$CLANG_PATH" &> /dev/null; then
    echo -e "${RED}Error: clang++ not found${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Compiler: $CLANG_PATH${NC}"
echo -e "${GREEN}✓ Google Benchmark: $BENCHMARK_DIR${NC}"
echo ""

# Create directories
mkdir -p "$BIN_DIR" "$OBJ_DIR" "$RESULTS_DIR"

# Function to compile benchmarks with specific optimization
compile_benchmarks() {
    local opt_flag="$1"
    local opt_label="${opt_flag#-}"
    
    echo -e "${CYAN}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${CYAN}Compiling benchmarks with $opt_flag (O$opt_label)${NC}"
    echo -e "${CYAN}═══════════════════════════════════════════════════════════${NC}"
    
    # Common compiler flags
    local common_flags=(
        "-std=$CXX_STANDARD"
        "-Wall"
        "-Wextra"
        "-Wpedantic"
        "$opt_flag"
        "-DNDEBUG"
    )
    
    # Google Benchmark flags
    local benchmark_flags=(
        "-DBENCHMARK_STATIC_DEFINE"
        "-I$BENCHMARK_DIR/include"
    )
    
    # Full paths to benchmark libraries
    local benchmark_lib="$BENCHMARK_DIR/build/src/libbenchmark.a"
    local benchmark_main_lib="$BENCHMARK_DIR/build/src/libbenchmark_main.a"
    
    local link_flags=(
        "$benchmark_lib"
        "$benchmark_main_lib"
        "-pthread"
    )
    
    # Compile matrix.cpp
    echo -e "  ${YELLOW}Compiling matrix.cpp...${NC}"
    local matrix_obj="$OBJ_DIR/matrix_${opt_label}.o"
    if "$CLANG_PATH" "${common_flags[@]}" -c "$SOURCE_DIR/matrix.cpp" -o "$matrix_obj"; then
        echo -e "    ${GREEN}✓ $matrix_obj${NC}"
    else
        echo -e "    ${RED}✗ Compilation failed!${NC}"
        return 1
    fi
    
    # Compile matrix_google_benchmark.cpp
    echo -e "  ${YELLOW}Compiling matrix_google_benchmark.cpp...${NC}"
    local bench_orig_obj="$OBJ_DIR/matrix_google_benchmark_${opt_label}.o"
    if "$CLANG_PATH" "${common_flags[@]}" "${benchmark_flags[@]}" -I"$SOURCE_DIR" -c "$SOURCE_DIR/matrix_google_benchmark.cpp" -o "$bench_orig_obj"; then
        echo -e "    ${GREEN}✓ $bench_orig_obj${NC}"
    else
        echo -e "    ${RED}✗ Compilation failed!${NC}"
        return 1
    fi
    
    # Link original benchmark
    echo -e "  ${YELLOW}Linking matrix_google_benchmark_${opt_label}...${NC}"
    local bench_orig_exe="$BIN_DIR/matrix_google_benchmark_${opt_label}"
    if "$CLANG_PATH" "${common_flags[@]}" "${benchmark_flags[@]}" "$matrix_obj" "$bench_orig_obj" "${link_flags[@]}" -o "$bench_orig_exe"; then
        echo -e "    ${GREEN}✓ $bench_orig_exe${NC}"
    else
        echo -e "    ${RED}✗ Linking failed!${NC}"
        return 1
    fi
    
    # Compile matrix_benchmark_advanced.cpp
    echo -e "  ${YELLOW}Compiling matrix_benchmark_advanced.cpp...${NC}"
    local bench_adv_obj="$OBJ_DIR/matrix_benchmark_advanced_${opt_label}.o"
    if "$CLANG_PATH" "${common_flags[@]}" "${benchmark_flags[@]}" -I"$SOURCE_DIR" -c "$SOURCE_DIR/matrix_benchmark_advanced.cpp" -o "$bench_adv_obj"; then
        echo -e "    ${GREEN}✓ $bench_adv_obj${NC}"
    else
        echo -e "    ${RED}✗ Compilation failed!${NC}"
        return 1
    fi
    
    # Link advanced benchmark
    echo -e "  ${YELLOW}Linking matrix_benchmark_advanced_${opt_label}...${NC}"
    local bench_adv_exe="$BIN_DIR/matrix_benchmark_advanced_${opt_label}"
    if "$CLANG_PATH" "${common_flags[@]}" "${benchmark_flags[@]}" "$matrix_obj" "$bench_adv_obj" "${link_flags[@]}" -o "$bench_adv_exe"; then
        echo -e "    ${GREEN}✓ $bench_adv_exe${NC}"
    else
        echo -e "    ${RED}✗ Linking failed!${NC}"
        return 1
    fi
    
    echo ""
    return 0
}

# Function to run benchmarks
run_benchmarks() {
    local opt_flag="$1"
    local exe_suffix="$2"
    local opt_label="${opt_flag#-}"
    
    echo -e "${GREEN}───────────────────────────────────────────────────────────${NC}"
    echo -e "${GREEN}Running benchmarks compiled with $opt_flag (O$opt_label)${NC}"
    echo -e "${GREEN}───────────────────────────────────────────────────────────${NC}"
    
    local timestamp=$(date +%Y%m%d_%H%M%S)
    local orig_csv="$RESULTS_DIR/benchmark_original_O${opt_label}_${timestamp}.csv"
    local adv_csv="$RESULTS_DIR/benchmark_advanced_O${opt_label}_${timestamp}.csv"
    
    local orig_exe="$BIN_DIR/matrix_google_benchmark_${exe_suffix}"
    local adv_exe="$BIN_DIR/matrix_benchmark_advanced_${exe_suffix}"
    
    # Run original benchmarks
    if [ -f "$orig_exe" ]; then
        echo -e "\n  ${CYAN}Running matrix_google_benchmark_${exe_suffix}...${NC}"
        echo -e "  ${CYAN}Saving to: $orig_csv${NC}"
        if "$orig_exe" --benchmark_min_time="$MIN_TIME" --benchmark_out="$orig_csv" --benchmark_out_format=csv 2>&1 | grep -E "Benchmark|Running"; then
            if [ -f "$orig_csv" ]; then
                local size=$(du -h "$orig_csv" | cut -f1)
                echo -e "  ${GREEN}✓ Saved ($size)${NC}"
            fi
        fi
    fi
    
    # Run advanced benchmarks
    if [ -f "$adv_exe" ]; then
        echo -e "\n  ${CYAN}Running matrix_benchmark_advanced_${exe_suffix}...${NC}"
        echo -e "  ${CYAN}Saving to: $adv_csv${NC}"
        if "$adv_exe" --benchmark_min_time="$MIN_TIME" --benchmark_out="$adv_csv" --benchmark_out_format=csv 2>&1 | grep -E "Benchmark|Running"; then
            if [ -f "$adv_csv" ]; then
                local size=$(du -h "$adv_csv" | cut -f1)
                echo -e "  ${GREEN}✓ Saved ($size)${NC}"
            fi
        fi
    fi
    
    echo ""
}

# Main execution
START_TIME=$(date +%s)

# Iterate through optimization levels
for opt_level in "${OPTIMIZATION_LEVELS[@]}"; do
    opt_label="${opt_level#-}"
    
    # Compile
    if compile_benchmarks "$opt_level"; then
        # Run
        run_benchmarks "$opt_level" "$opt_label"
    else
        echo -e "${RED}Skipping benchmarks for $opt_level due to compilation failure${NC}"
    fi
done

END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))
MINUTES=$((DURATION / 60))
SECONDS=$((DURATION % 60))

# Summary
echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
echo -e "${GREEN}║   Benchmark Optimization Analysis Complete ✓              ║${NC}"
echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
echo ""
echo -e "${YELLOW}Total execution time: ${MINUTES}m ${SECONDS}s${NC}"
echo -e "${YELLOW}Results saved to: $RESULTS_DIR/${NC}"
echo ""

# List generated files
echo -e "${CYAN}Generated result files:${NC}"
for file in "$RESULTS_DIR"/benchmark_*_O*.csv; do
    if [ -f "$file" ]; then
        size=$(du -h "$file" | cut -f1)
        echo -e "  ${GREEN}•${NC} $(basename "$file") ($size)"
    fi
done
echo ""

# Next steps
echo -e "${YELLOW}Next steps:${NC}"
echo -e "  ${CYAN}1. Analyze results: python3 analyze_optimization_levels.py results/benchmark_*_O*.csv${NC}"
echo -e "  ${CYAN}2. Compare optimization levels for performance impact${NC}"
echo ""
