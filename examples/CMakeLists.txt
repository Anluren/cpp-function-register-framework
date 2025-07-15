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
    
    # Create modern C++17 demo executable
    add_executable(modern_demo modern_demo.cpp)
    target_link_libraries(modern_demo function_register_core)
    target_include_directories(modern_demo 
        PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    
    # Install modern demo
    install(TARGETS modern_demo
        RUNTIME DESTINATION bin
    )
    
    # Create simple modern C++17 demo executable
    add_executable(simple_modern_demo simple_modern_demo.cpp)
    target_link_libraries(simple_modern_demo function_register_core)
    target_include_directories(simple_modern_demo 
        PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    
    # Install simple modern demo
    install(TARGETS simple_modern_demo
        RUNTIME DESTINATION bin
    )
    
    # Create modular demo executable (demonstrates separate .cpp files for each function group)
    add_executable(modular_demo 
        modular_demo.cpp
        math_function_group.cpp
        string_function_group.cpp
        utility_function_group.cpp
    )
    target_link_libraries(modular_demo function_register_core)
    target_include_directories(modular_demo 
        PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    
    # Install modular demo
    install(TARGETS modular_demo
        RUNTIME DESTINATION bin
    )
    
    # Create performance benchmark executable
    add_executable(performance_benchmark performance_benchmark.cpp)
    target_link_libraries(performance_benchmark function_register_core)
    target_include_directories(performance_benchmark 
        PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    
    # Install performance benchmark
    install(TARGETS performance_benchmark
        RUNTIME DESTINATION bin
    )
    
    # Create performance comparison executable
    add_executable(performance_comparison performance_comparison.cpp)
    target_link_libraries(performance_comparison function_register_core)
    target_include_directories(performance_comparison 
        PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    
    # Install performance comparison
    install(TARGETS performance_comparison
        RUNTIME DESTINATION bin
    )
    
    # Create constrained function demo executable
    add_executable(constrained_function_demo constrained_function_demo.cpp)
    target_link_libraries(constrained_function_demo function_register_core)
    target_include_directories(constrained_function_demo 
        PRIVATE ${CMAKE_SOURCE_DIR}/include
    )
    
    # Install constrained function demo
    install(TARGETS constrained_function_demo
        RUNTIME DESTINATION bin
    )
endif()
