# Minimum CMake version required
cmake_minimum_required(VERSION 3.14)

# Project name
project(COG_BIAS_MODEL)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

find_package(Eigen3 REQUIRED)
# Include directories
include_directories(arch/node/headr)
include_directories(/opt/homebrew/opt/googletest/include)
include_directories(/opt/homebrew/opt/eigen/include/eigen3)

# src test files
file(GLOB NODE_SRC "./arch/node/src/*.cpp")
file(GLOB NODE_TEST_SRC "./arch/node/test/*.cpp")

# make executable
add_executable(run_tests ${NODE_SRC} ${NODE_TEST_SRC}
        arch/layer/headr/layer.h
        arch/node/test/node_test_LSTM.cpp
        arch/layer/src/layer.cpp
        arch/layer/test/layer_test.cpp)

# Link Google Test libraries
target_link_libraries(run_tests
        /opt/homebrew/opt/googletest/lib/libgtest.a
        /opt/homebrew/opt/googletest/lib/libgtest_main.a
        pthread
)
