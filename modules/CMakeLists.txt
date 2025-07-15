# Function Register Framework - Modules
# This directory contains modular implementations of the function registration framework
# Each module demonstrates a different approach or use case

cmake_minimum_required(VERSION 3.16)

# Set C++17 as the minimum standard for all modules
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add all module subdirectories
add_subdirectory(legacy)
add_subdirectory(modern)
add_subdirectory(optimized)
# TODO: Implement remaining modules
# add_subdirectory(standard)
# add_subdirectory(multi_module)
# add_subdirectory(constrained)

# Create a target that builds all modules
add_custom_target(all_modules)
add_dependencies(all_modules 
    func_register_legacy
    func_register_modern
    func_register_optimized
    # TODO: Add remaining modules when implemented
    # func_register_standard
    # func_register_multi_module
    # func_register_constrained
)

# Create module tests target
add_custom_target(test_modules
    COMMAND echo "Running module tests..."
    DEPENDS all_modules
)
