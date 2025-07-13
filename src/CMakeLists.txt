# Core library source files
set(CORE_SOURCES
    function_group.cpp
    function_registry.cpp
)

# Create the core library
add_library(function_register_core STATIC ${CORE_SOURCES})

# Set include directories for the library
target_include_directories(function_register_core 
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Set properties
set_target_properties(function_register_core PROPERTIES
    OUTPUT_NAME "function_register"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    EXPORT_NAME function_register
)

# Create alias for consistent naming
add_library(FunctionRegisterFramework::function_register ALIAS function_register_core)

# Install the library
install(TARGETS function_register_core
    EXPORT FunctionRegisterFrameworkTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)
