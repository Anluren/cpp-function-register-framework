cmake_minimum_required(VERSION 3.10)
project(FunctionRegisterFramework VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -DDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add subdirectories
add_subdirectory(src)
add_subdirectory(examples)

# Options
option(BUILD_EXAMPLES "Build example programs" ON)

# Print build information
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build examples: ${BUILD_EXAMPLES}")

# Installation settings
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
set(INCLUDE_INSTALL_DIR include)
set(LIB_INSTALL_DIR lib)
set(CONFIG_INSTALL_DIR lib/cmake/FunctionRegisterFramework)

# Install headers
install(DIRECTORY include/ DESTINATION ${INCLUDE_INSTALL_DIR}
        FILES_MATCHING PATTERN "*.h")

# Export targets
install(EXPORT FunctionRegisterFrameworkTargets
        FILE FunctionRegisterFrameworkTargets.cmake
        NAMESPACE FunctionRegisterFramework::
        DESTINATION ${CONFIG_INSTALL_DIR})

# Configure package config file
include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/FunctionRegisterFrameworkConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/FunctionRegisterFrameworkConfig.cmake"
    INSTALL_DESTINATION ${CONFIG_INSTALL_DIR}
    PATH_VARS INCLUDE_INSTALL_DIR
)

# Generate version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/FunctionRegisterFrameworkConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Install config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/FunctionRegisterFrameworkConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/FunctionRegisterFrameworkConfigVersion.cmake"
    DESTINATION ${CONFIG_INSTALL_DIR}
)
