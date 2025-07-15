/**
 * Basic Math Module for Multi-Module Demo
 * Implements basic arithmetic functions with simple algorithms
 */

#include "../include/multi_module_registry.h"
#include <iostream>

namespace BasicMath {
    
    int add(int a, int b) {
        std::cout << "[BasicMath] Computing " << a << " + " << b << " = " << (a + b) << std::endl;
        return a + b;
    }
    
    int multiply(int a, int b) {
        std::cout << "[BasicMath] Computing " << a << " * " << b << " = " << (a * b) << std::endl;
        return a * b;
    }
    
    int square(int n) {
        int result = n * n;
        std::cout << "[BasicMath] Computing " << n << "^2 = " << result << std::endl;
        return result;
    }
    
    int abs(int n) {
        int result = n < 0 ? -n : n;
        std::cout << "[BasicMath] Computing abs(" << n << ") = " << result << std::endl;
        return result;
    }
    
    // Registration function
    void register_functions() {
        auto& registry = MultiModuleFunctionRegistry::instance();
        
        registry.register_function("BasicMath", StandardFunctionId::ADD, 
                                 "Simple addition", add);
        registry.register_function("BasicMath", StandardFunctionId::MULTIPLY, 
                                 "Simple multiplication", multiply);
        registry.register_function("BasicMath", StandardFunctionId::SQUARE, 
                                 "Square using simple multiplication", square);
        registry.register_function("BasicMath", StandardFunctionId::ABS, 
                                 "Absolute value using conditional", abs);
    }
}

// Static registration
namespace {
    struct BasicMathRegistrar {
        BasicMathRegistrar() {
            BasicMath::register_functions();
        }
    };
    static BasicMathRegistrar basic_math_registrar;
}
