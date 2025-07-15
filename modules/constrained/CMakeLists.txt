# Constrained Function Group Module  
# Template-based constraint validation with compile-time checks

# Set C++ standard for template features
set(CMAKE_CXX_STANDARD 17)

# Constrained module sources
set(CONSTRAINED_SOURCES
    constrained_function_group.cpp
    constrained_math_functions.cpp
    constrained_string_functions.cpp
    constrained_utility_functions.cpp
)

# Constrained module headers
set(CONSTRAINED_HEADERS
    constrained_function_group.h
    constrained_math_functions.h
    constrained_string_functions.h
    constrained_utility_functions.h
)

# Create constrained function group library
add_library(function_register_constrained STATIC ${CONSTRAINED_SOURCES})

# Link with core library
target_link_libraries(function_register_constrained function_register_core)

# Set include directories
target_include_directories(function_register_constrained 
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)

# Install library
install(TARGETS function_register_constrained
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install headers
install(FILES ${CONSTRAINED_HEADERS}
    DESTINATION include/function_register/constrained
)
