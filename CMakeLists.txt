cmake_minimum_required(VERSION 3.20)
project(OperatorOverloading
    VERSION 1.0
    DESCRIPTION "Operator Overloading Analysis - Efficient C++ Programming"
    LANGUAGES CXX
)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")

# Compiler flags for different configurations
if(MSVC)
    # MSVC specific flags
    set(CMAKE_CXX_FLAGS_DEBUG "/W4 /Zi /Od /RTC1")
    set(CMAKE_CXX_FLAGS_RELEASE "/W4 /O2 /DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/W4 /O2 /Zi")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "/W4 /Os /DNDEBUG")
else()
    # GCC/Clang flags (including clang++ on Windows with LLVM)
    set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Wextra -g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-Wall -Wextra -O2 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Wall -Wextra -O2 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "-Wall -Wextra -Os -DNDEBUG")
endif()

# Add subdirectories
add_subdirectory(src)

# Optional: Add documentation
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/docs")
    message(STATUS "Documentation directory found")
endif()

# Display configuration summary
message(STATUS "")
message(STATUS "========== Build Configuration ==========")
message(STATUS "Project: ${PROJECT_NAME}")
message(STATUS "Version: ${PROJECT_VERSION}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "=========================================")
message(STATUS "")
