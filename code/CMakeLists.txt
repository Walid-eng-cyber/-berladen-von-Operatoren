cmake_minimum_required(VERSION 3.20)

# ============================================================
# Matrix Libraries (Original + V1/V2/V3)
# ============================================================

# Original Matrix library from matrix.cpp
add_library(matrix_lib STATIC matrix.cpp)
target_compile_features(matrix_lib PRIVATE cxx_std_17)
target_include_directories(matrix_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
    target_compile_options(matrix_lib PRIVATE /W4)
else()
    target_compile_options(matrix_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Created Matrix library: matrix_lib")

# MatrixV1: Basic copy semantics only
add_library(matrix_v1_lib STATIC matrix_v1.cpp)
target_compile_features(matrix_v1_lib PRIVATE cxx_std_17)
target_include_directories(matrix_v1_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
    target_compile_options(matrix_v1_lib PRIVATE /W4)
else()
    target_compile_options(matrix_v1_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Created Matrix V1 library: matrix_v1_lib")

# MatrixV2: Copy + Move semantics
add_library(matrix_v2_lib STATIC matrix_v2.cpp)
target_compile_features(matrix_v2_lib PRIVATE cxx_std_17)
target_include_directories(matrix_v2_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
    target_compile_options(matrix_v2_lib PRIVATE /W4)
else()
    target_compile_options(matrix_v2_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Created Matrix V2 library: matrix_v2_lib")

# MatrixV3: Move semantics + Compound operators
add_library(matrix_v3_lib STATIC matrix_v3.cpp)
target_compile_features(matrix_v3_lib PRIVATE cxx_std_17)
target_include_directories(matrix_v3_lib PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

if(MSVC)
    target_compile_options(matrix_v3_lib PRIVATE /W4)
else()
    target_compile_options(matrix_v3_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

message(STATUS "Created Matrix V3 library: matrix_v3_lib")

# ============================================================
# Executables
# ============================================================

# RVO example executable
add_executable(rvo_example RVO.cpp)
target_compile_features(rvo_example PRIVATE cxx_std_17)

# RVO without optimization example
add_executable(rvo_no_optimization RVO_NO_OPTIMIZATION.cpp)
target_compile_features(rvo_no_optimization PRIVATE cxx_std_17)

# Matrix demo executable
add_executable(matrix_demo matrix_demo.cpp)
target_compile_features(matrix_demo PRIVATE cxx_std_17)
target_link_libraries(matrix_demo PRIVATE matrix_lib)

# Comparison benchmark: V1 vs V2 vs V3
add_executable(comparison_benchmark comparison_benchmark.cpp)
target_compile_features(comparison_benchmark PRIVATE cxx_std_17)
target_link_libraries(comparison_benchmark PRIVATE matrix_v1_lib matrix_v2_lib matrix_v3_lib)

# Set output directory for executables
set_target_properties(rvo_example rvo_no_optimization matrix_demo comparison_benchmark
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Add include directories if needed
target_include_directories(rvo_example PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(rvo_no_optimization PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

# Optional: Add compiler warnings
if(MSVC)
    target_compile_options(rvo_example PRIVATE /W4)
    target_compile_options(rvo_no_optimization PRIVATE /W4)
    target_compile_options(matrix_demo PRIVATE /W4)
else()
    target_compile_options(rvo_example PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(rvo_no_optimization PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(matrix_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Print configuration
message(STATUS "Configured executables:")
message(STATUS "  - rvo_example")
message(STATUS "  - rvo_no_optimization")
message(STATUS "  - matrix_demo")
message(STATUS "  - comparison_benchmark (V1 vs V2 vs V3)")

# Optional: Add compiler warnings to comparison_benchmark
if(MSVC)
    target_compile_options(comparison_benchmark PRIVATE /W4)
else()
    target_compile_options(comparison_benchmark PRIVATE -Wall -Wextra -Wpedantic)
endif()
