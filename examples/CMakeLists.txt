if(BUILD_EXAMPLES)
    # Example function groups
    set(EXAMPLE_FUNCTION_SOURCES
        math_functions.cpp
        string_functions.cpp
        utility_functions.cpp
    )
    
    # Create example function groups library
    add_library(example_functions STATIC ${EXAMPLE_FUNCTION_SOURCES})
    
    # Link with core library
    target_link_libraries(example_functions function_register_core)
    
    # Set include directories
    target_include_directories(example_functions 
        PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    
    # Main demo executable
    add_executable(func_register_demo 
        main.cpp 
        math_functions.cpp
        string_functions.cpp
        utility_functions.cpp
    )
    
    # Link with core library
    target_link_libraries(func_register_demo function_register_core)
    
    # Set include directories for the executable
    target_include_directories(func_register_demo 
        PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    
    # Install demo executable
    install(TARGETS func_register_demo
        RUNTIME DESTINATION bin
    )
    
    # Create a simple example executable (minimal usage)
    add_executable(simple_example simple_example.cpp)
    target_link_libraries(simple_example function_register_core)
    target_include_directories(simple_example 
        PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    
    # Install simple example
    install(TARGETS simple_example
        RUNTIME DESTINATION bin
    )
endif()
