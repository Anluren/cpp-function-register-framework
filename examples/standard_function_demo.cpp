#include "../include/standard_function_registry.h"
#include <iostream>
#include <string>

/**
 * Demo of the Standard Function Registry System
 * 
 * This demonstrates:
 * 1. Predefined function interface
 * 2. Modules implementing subsets of functions
 * 3. Centralized registry with type safety
 * 4. Runtime checking of function availability
 */

int main() {
    std::cout << "=== Standard Function Registry Demo ===" << std::endl;
    std::cout << "Demonstrating predefined function interfaces with modular implementation" << std::endl << std::endl;
    
    auto& registry = StandardFunctionRegistry::instance();
    
    // Show what modules are registered
    std::cout << "=== Registered Modules ===" << std::endl;
    auto modules = registry.get_modules();
    for (const auto& module : modules) {
        std::cout << "Module: " << module << std::endl;
        auto functions = registry.get_module_functions(module);
        std::cout << "  Functions: ";
        for (const auto& func : functions) {
            std::cout << to_string(func) << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    // Show all available functions
    std::cout << "=== All Available Functions ===" << std::endl;
    auto all_functions = registry.get_all_functions();
    for (const auto& func : all_functions) {
        std::cout << "- " << to_string(func) << std::endl;
    }
    std::cout << std::endl;
    
    // Test math functions (if available)
    std::cout << "=== Testing Math Functions ===" << std::endl;
    
    if (registry.has_function(StandardFunction::ADD_INT)) {
        auto result = registry.add_int(15, 25);
        if (result) {
            std::cout << "ADD_INT(15, 25) = " << *result << std::endl;
        }
    } else {
        std::cout << "ADD_INT function not available" << std::endl;
    }
    
    if (registry.has_function(StandardFunction::MULTIPLY_DOUBLE)) {
        auto result = registry.call_function<double>(StandardFunction::MULTIPLY_DOUBLE, 3.14, 2.0);
        if (result) {
            std::cout << "MULTIPLY_DOUBLE(3.14, 2.0) = " << *result << std::endl;
        }
    } else {
        std::cout << "MULTIPLY_DOUBLE function not available" << std::endl;
    }
    
    // Test string functions (if available)
    std::cout << std::endl << "=== Testing String Functions ===" << std::endl;
    
    if (registry.has_function(StandardFunction::TO_UPPER)) {
        auto result = registry.to_upper("hello world");
        if (result) {
            std::cout << "TO_UPPER(\"hello world\") = \"" << *result << "\"" << std::endl;
        }
    } else {
        std::cout << "TO_UPPER function not available" << std::endl;
    }
    
    if (registry.has_function(StandardFunction::CONCAT)) {
        auto result = registry.call_function<std::string>(StandardFunction::CONCAT, "Hello", "World");
        if (result) {
            std::cout << "CONCAT(\"Hello\", \"World\") = \"" << *result << "\"" << std::endl;
        }
    } else {
        std::cout << "CONCAT function not available" << std::endl;
    }
    
    // Test utility functions (if available)
    std::cout << std::endl << "=== Testing Utility Functions ===" << std::endl;
    
    if (registry.has_function(StandardFunction::IS_EVEN)) {
        auto result = registry.is_even(42);
        if (result) {
            std::cout << "IS_EVEN(42) = " << (*result ? "true" : "false") << std::endl;
        }
    } else {
        std::cout << "IS_EVEN function not available" << std::endl;
    }
    
    if (registry.has_function(StandardFunction::FIBONACCI)) {
        auto result = registry.call_function<int>(StandardFunction::FIBONACCI, 10);
        if (result) {
            std::cout << "FIBONACCI(10) = " << *result << std::endl;
        }
    } else {
        std::cout << "FIBONACCI function not available" << std::endl;
    }
    
    // Test functions that are NOT implemented by any module
    std::cout << std::endl << "=== Testing Unimplemented Functions ===" << std::endl;
    
    if (registry.has_function(StandardFunction::HTTP_GET)) {
        std::cout << "HTTP_GET function is available" << std::endl;
    } else {
        std::cout << "HTTP_GET function not implemented by any module" << std::endl;
    }
    
    if (registry.has_function(StandardFunction::READ_FILE)) {
        std::cout << "READ_FILE function is available" << std::endl;
    } else {
        std::cout << "READ_FILE function not implemented by any module" << std::endl;
    }
    
    std::cout << std::endl << "=== Demo Summary ===" << std::endl;
    std::cout << "This demonstrates how:" << std::endl;
    std::cout << "1. You define a standard interface (StandardFunction enum)" << std::endl;
    std::cout << "2. Each module implements only the functions it supports" << std::endl;
    std::cout << "3. Runtime checking ensures type safety and availability" << std::endl;
    std::cout << "4. Easy to extend by adding new StandardFunction values" << std::endl;
    std::cout << "5. Modules are independent and can be loaded/unloaded" << std::endl;
    
    return 0;
}
