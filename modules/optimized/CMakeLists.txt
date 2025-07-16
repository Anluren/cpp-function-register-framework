# Optimized Function Registration Module
# Focuses on performance with minimal overhead

cmake_minimum_required(VERSION 3.16)
project(func_register_optimized)

# Optimizations with C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")

# Find required packages
find_package(Threads REQUIRED)

# Create optimized module library
add_library(func_register_optimized STATIC
    optimized_registry.cpp
    optimized_math.cpp
    optimized_string.cpp
    optimized_utility.cpp
)

target_include_directories(func_register_optimized PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Link with threading library
target_link_libraries(func_register_optimized Threads::Threads)

# Create optimized demo executable
add_executable(optimized_demo optimized_demo.cpp)
target_link_libraries(optimized_demo func_register_optimized)

# Installation
install(TARGETS func_register_optimized DESTINATION lib)
install(FILES 
    optimized_registry.h
    optimized_function_group.h
    DESTINATION include/func_register/optimized
)
