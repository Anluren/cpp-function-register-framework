#include "simple_modern_function_group.h"
#include <iostream>
#include <vector>

// Utility function implementations
void hello_world() {
    std::cout << "Hello, World!" << std::endl;
}

int fibonacci(int n) {
    if (n <= 1) return n;
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

bool is_even(int n) {
    return n % 2 == 0;
}

int max_of_two(int a, int b) {
    return (a > b) ? a : b;
}

// Function to register utility functions
void register_utility_functions() {
    auto utility_group = std::make_unique<SimpleFunctionGroup>("Utility Functions");
    
    // Register function pointers
    utility_group->add("hello_world", hello_world);
    utility_group->add("fibonacci", fibonacci);
    utility_group->add("is_even", is_even);
    utility_group->add("max", max_of_two);
    
    // Register lambdas for additional utility operations
    utility_group->add("abs", [](int x) { 
        return (x < 0) ? -x : x; 
    });
    
    utility_group->add("min", [](int a, int b) { 
        return (a < b) ? a : b; 
    });
    
    utility_group->add("random_bool", []() { 
        return true; // Simple implementation
    });
    
    utility_group->add("clamp", [](int value, int /*min_val*/, int /*max_val*/) {
        // Note: This is a 3-argument function, but our current implementation 
        // only supports up to 2 arguments. This is just for demonstration.
        // In a real implementation, you would extend the framework to support more arguments.
        return value;
    });
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::UTILITY_FUNCTIONS, std::move(utility_group));
}
