# Standard Function Registration Module
# Clean, simple C++17 implementation using standard library patterns

cmake_minimum_required(VERSION 3.16)
project(func_register_standard)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create standard module library
add_library(func_register_standard STATIC
    standard_registry.cpp
    standard_functions.cpp
)

target_include_directories(func_register_standard PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Create standard demo executable
add_executable(standard_demo standard_demo.cpp)
target_link_libraries(standard_demo func_register_standard)

# Installation
install(TARGETS func_register_standard DESTINATION lib)
install(FILES 
    standard_registry.h
    DESTINATION include/func_register/standard
)
