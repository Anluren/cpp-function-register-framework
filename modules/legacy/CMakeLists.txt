# Legacy Function Registration Module
# Simple, basic function registration for backwards compatibility

cmake_minimum_required(VERSION 3.16)
project(func_register_legacy)

# Set C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Create legacy module library
add_library(func_register_legacy STATIC
    legacy_registry.cpp
    legacy_math.cpp
    legacy_string.cpp
    legacy_utility.cpp
    legacy_init.cpp
)

target_include_directories(func_register_legacy PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Create legacy demo executable
add_executable(legacy_module_demo legacy_demo.cpp)
target_link_libraries(legacy_module_demo func_register_legacy)

# Installation
install(TARGETS func_register_legacy DESTINATION lib)
install(FILES 
    legacy_registry.h
    legacy_function_group.h
    DESTINATION include/func_register/legacy
)
