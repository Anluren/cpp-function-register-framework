# Multi-Module Registry Module
# Multiple implementations per function with caller-controlled selection

# Set C++ standard for modern features
set(CMAKE_CXX_STANDARD 17)

# Multi-module registry sources
set(MULTI_MODULE_SOURCES
    multi_module_registry.cpp
    math_basic_module.cpp
    math_optimized_module.cpp
    math_debug_module.cpp
)

# Multi-module registry headers
set(MULTI_MODULE_HEADERS
    multi_module_registry.h
    math_basic_module.h
    math_optimized_module.h
    math_debug_module.h
)

# Create multi-module registry library
add_library(function_register_multi_module STATIC ${MULTI_MODULE_SOURCES})

# Link with core library
target_link_libraries(function_register_multi_module function_register_core)

# Set include directories
target_include_directories(function_register_multi_module 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# Install library
install(TARGETS function_register_multi_module
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(FILES ${MULTI_MODULE_HEADERS}
    DESTINATION include/function_register/multi_module
)
