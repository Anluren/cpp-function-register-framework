#include "../include/standard_function_registry.h"
#include <iostream>
#include <algorithm>

/**
 * Math Module - implements only math-related standard functions
 */

namespace math_module {

// Implementation of standard math functions
int add_int_impl(int a, int b) {
    return a + b;
}

double multiply_double_impl(double a, double b) {
    return a * b;
}

int square_int_impl(int x) {
    return x * x;
}

double power_double_impl(double base, double exponent) {
    double result = 1.0;
    for (int i = 0; i < static_cast<int>(exponent); ++i) {
        result *= base;
    }
    return result;
}

// Module registration function
void register_math_module() {
    ModuleRegistrar registrar("MathModule");
    
    std::cout << "Registering Math Module functions..." << std::endl;
    
    // Register only the math functions this module supports
    registrar.register_function(StandardFunction::ADD_INT, add_int_impl);
    registrar.register_function(StandardFunction::MULTIPLY_DOUBLE, multiply_double_impl);
    registrar.register_function(StandardFunction::SQUARE_INT, square_int_impl);
    registrar.register_function(StandardFunction::POWER_DOUBLE, power_double_impl);
    
    // Note: This module does NOT implement string, file, or network functions
    // Other modules can implement those
    
    std::cout << "Math Module registered 4 functions." << std::endl;
}

} // namespace math_module

// Automatic registration
namespace {
    struct MathModuleAutoRegistration {
        MathModuleAutoRegistration() {
            math_module::register_math_module();
        }
    };
    static MathModuleAutoRegistration auto_register;
}
