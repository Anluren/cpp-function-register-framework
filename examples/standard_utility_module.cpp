#include "../include/standard_function_registry.h"
#include <iostream>

/**
 * Utility Module - implements only utility-related standard functions
 */

namespace utility_module {

// Implementation of standard utility functions
bool is_even_impl(int n) {
    return n % 2 == 0;
}

int fibonacci_impl(int n) {
    if (n <= 1) return n;
    
    int a = 0, b = 1;
    for (int i = 2; i <= n; ++i) {
        int temp = a + b;
        a = b;
        b = temp;
    }
    return b;
}

int random_int_impl() {
    return 42; // Simple implementation for demo
}

void sleep_ms_impl(int milliseconds) {
    std::cout << "Sleeping for " << milliseconds << " ms..." << std::endl;
    // In real implementation, you would use std::this_thread::sleep_for
}

// Module registration function
void register_utility_module() {
    ModuleRegistrar registrar("UtilityModule");
    
    std::cout << "Registering Utility Module functions..." << std::endl;
    
    // Register only the utility functions this module supports
    registrar.register_function(StandardFunction::IS_EVEN, is_even_impl);
    registrar.register_function(StandardFunction::FIBONACCI, fibonacci_impl);
    registrar.register_function(StandardFunction::RANDOM_INT, random_int_impl);
    registrar.register_function(StandardFunction::SLEEP_MS, sleep_ms_impl);
    
    // Note: This module does NOT implement math, string, file, or network functions
    
    std::cout << "Utility Module registered 4 functions." << std::endl;
}

} // namespace utility_module

// Automatic registration
namespace {
    struct UtilityModuleAutoRegistration {
        UtilityModuleAutoRegistration() {
            utility_module::register_utility_module();
        }
    };
    static UtilityModuleAutoRegistration auto_register;
}
