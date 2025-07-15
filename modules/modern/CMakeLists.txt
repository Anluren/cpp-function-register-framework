# Modern Function Registration Module
# Uses C++17+ features like lambdas, smart pointers, and type safety

cmake_minimum_required(VERSION 3.16)
project(func_register_modern)

# Require C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create modern module library
add_library(func_register_modern STATIC
    modern_registry.cpp
    modern_math.cpp
    modern_string.cpp
    modern_utility.cpp
)

target_include_directories(func_register_modern PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Create modern demo executable
add_executable(modern_demo modern_demo.cpp)
target_link_libraries(modern_demo func_register_modern)

# Installation
install(TARGETS func_register_modern DESTINATION lib)
install(FILES 
    modern_registry.h
    modern_function_group.h
    DESTINATION include/func_register/modern
)
