/**
 * Multi-Module Registry Demo
 * 
 * Demonstrates how multiple modules can implement the same function,
 * and how callers can choose which implementation to use.
 */

#include "../include/multi_module_registry.h"
#include <iostream>
#include <string>
#include <vector>

// Forward declare registration functions
namespace BasicMath { void register_functions(); }
namespace OptimizedMath { void register_functions(); }
namespace DebugMath { void register_functions(); }

int main() {
    std::cout << "=== Multi-Module Function Registry Demo ===" << std::endl;
    std::cout << "Demonstrating multiple implementations of the same function\n" << std::endl;
    
    // Get registry instance
    auto& registry = MultiModuleFunctionRegistry::instance();
    
    // Functions are automatically registered via static initialization
    std::cout << "Functions automatically registered during static initialization\n" << std::endl;
    
    // Show overview of all implementations
    registry.show_implementation_overview();
    
    std::cout << "\n=== Testing Multiple Implementations ===" << std::endl;
    
    // Test ADD function with different modules
    std::cout << "\n--- Testing ADD function (5 + 3) ---" << std::endl;
    
    // Try BasicMath implementation
    auto result1 = registry.call_function<int>("BasicMath", StandardFunctionId::ADD, 5, 3);
    if (result1) {
        std::cout << "BasicMath result: " << *result1 << std::endl;
    }
    
    // Try OptimizedMath implementation
    auto result2 = registry.call_function<int>("OptimizedMath", StandardFunctionId::ADD, 5, 3);
    if (result2) {
        std::cout << "OptimizedMath result: " << *result2 << std::endl;
    }
    
    // Try DebugMath implementation
    auto result3 = registry.call_function<int>("DebugMath", StandardFunctionId::ADD, 5, 3);
    if (result3) {
        std::cout << "DebugMath result: " << *result3 << std::endl;
    }
    
    // Test MULTIPLY function with different modules
    std::cout << "\n--- Testing MULTIPLY function (4 * 8) ---" << std::endl;
    
    auto mult1 = registry.call_function<int>("BasicMath", StandardFunctionId::MULTIPLY, 4, 8);
    if (mult1) {
        std::cout << "BasicMath result: " << *mult1 << std::endl;
    }
    
    auto mult2 = registry.call_function<int>("OptimizedMath", StandardFunctionId::MULTIPLY, 4, 8);
    if (mult2) {
        std::cout << "OptimizedMath result: " << *mult2 << std::endl;
    }
    
    auto mult3 = registry.call_function<int>("DebugMath", StandardFunctionId::MULTIPLY, 4, 8);
    if (mult3) {
        std::cout << "DebugMath result: " << *mult3 << std::endl;
    }
    
    // Test ABS function with different modules
    std::cout << "\n--- Testing ABS function (-15) ---" << std::endl;
    
    auto abs1 = registry.call_function<int>("BasicMath", StandardFunctionId::ABS, -15);
    if (abs1) {
        std::cout << "BasicMath result: " << *abs1 << std::endl;
    }
    
    auto abs2 = registry.call_function<int>("OptimizedMath", StandardFunctionId::ABS, -15);
    if (abs2) {
        std::cout << "OptimizedMath result: " << *abs2 << std::endl;
    }
    
    auto abs3 = registry.call_function<int>("DebugMath", StandardFunctionId::ABS, -15);
    if (abs3) {
        std::cout << "DebugMath result: " << *abs3 << std::endl;
    }
    
    // Test function that's only available in some modules
    std::cout << "\n--- Testing SQRT function (only in OptimizedMath) ---" << std::endl;
    
    auto sqrt1 = registry.call_function<double>("BasicMath", StandardFunctionId::SQRT, 16);
    if (sqrt1) {
        std::cout << "BasicMath sqrt result: " << *sqrt1 << std::endl;
    } else {
        std::cout << "BasicMath doesn't implement SQRT" << std::endl;
    }
    
    auto sqrt2 = registry.call_function<double>("OptimizedMath", StandardFunctionId::SQRT, 16);
    if (sqrt2) {
        std::cout << "OptimizedMath sqrt result: " << *sqrt2 << std::endl;
    } else {
        std::cout << "OptimizedMath doesn't implement SQRT" << std::endl;
    }
    
    // Test call_function_any (uses first available implementation)
    std::cout << "\n--- Testing call_function_any (uses first available) ---" << std::endl;
    
    auto any_result = registry.call_function_any<int>(StandardFunctionId::SQUARE, 7);
    if (any_result) {
        std::cout << "Square(7) from any module: " << *any_result << std::endl;
    }
    
    // Show which modules implement which functions
    std::cout << "\n=== Module Analysis ===" << std::endl;
    
    auto all_modules = registry.get_all_modules();
    for (const auto& module : all_modules) {
        std::cout << "\n" << module << " implements: ";
        auto functions = registry.get_module_functions(module);
        for (size_t i = 0; i < functions.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << to_string(functions[i]);
        }
        std::cout << std::endl;
    }
    
    // Show which modules provide each function
    std::cout << "\n=== Function Provider Analysis ===" << std::endl;
    
    std::vector<StandardFunctionId> test_functions = {
        StandardFunctionId::ADD,
        StandardFunctionId::MULTIPLY,
        StandardFunctionId::SQUARE,
        StandardFunctionId::ABS,
        StandardFunctionId::SQRT
    };
    
    for (auto func_id : test_functions) {
        auto providers = registry.get_function_providers(func_id);
        std::cout << to_string(func_id) << " is implemented by: ";
        for (size_t i = 0; i < providers.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << providers[i];
        }
        std::cout << " (" << providers.size() << " modules)" << std::endl;
    }
    
    // Test error handling
    std::cout << "\n=== Error Handling Demo ===" << std::endl;
    
    // Try to call non-existent function
    auto no_result = registry.call_function<int>("NonExistentModule", StandardFunctionId::ADD, 1, 2);
    if (!no_result) {
        std::cout << "✓ Correctly returned nullopt for non-existent module" << std::endl;
    }
    
    // Try to call function from module that doesn't implement it
    auto no_sqrt = registry.call_function<double>("BasicMath", StandardFunctionId::SQRT, 25);
    if (!no_sqrt) {
        std::cout << "✓ Correctly returned nullopt for unimplemented function" << std::endl;
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "This demo shows how:" << std::endl;
    std::cout << "1. Multiple modules can implement the same function ID" << std::endl;
    std::cout << "2. Callers can choose which module's implementation to use" << std::endl;
    std::cout << "3. The registry tracks which modules implement which functions" << std::endl;
    std::cout << "4. Error handling works when modules/functions don't exist" << std::endl;
    std::cout << "5. Functions can be called from any available module" << std::endl;
    
    return 0;
}
