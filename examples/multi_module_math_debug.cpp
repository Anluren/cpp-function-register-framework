/**
 * Debug Math Module for Multi-Module Demo
 * Implements arithmetic functions with detailed logging and validation
 */

#include "../include/multi_module_registry.h"
#include <iostream>
#include <stdexcept>
#include <iomanip>

namespace DebugMath {
    
    int add(int a, int b) {
        std::cout << "[DebugMath] Input validation: a=" << a << ", b=" << b << std::endl;
        
        // Check for overflow
        if (a > 0 && b > 0 && a > INT_MAX - b) {
            throw std::overflow_error("Addition would overflow");
        }
        if (a < 0 && b < 0 && a < INT_MIN - b) {
            throw std::underflow_error("Addition would underflow");
        }
        
        int result = a + b;
        std::cout << "[DebugMath] Safe addition: " << a << " + " << b << " = " << result 
                  << " (no overflow)" << std::endl;
        return result;
    }
    
    int multiply(int a, int b) {
        std::cout << "[DebugMath] Verbose multiply: " << a << " * " << b << std::endl;
        
        // Check for overflow
        if (a != 0 && b != 0) {
            if (a > 0 && b > 0 && a > INT_MAX / b) {
                throw std::overflow_error("Multiplication would overflow");
            }
            if (a < 0 && b < 0 && a < INT_MAX / b) {
                throw std::overflow_error("Multiplication would overflow");
            }
            if ((a > 0 && b < 0 && b < INT_MIN / a) || 
                (a < 0 && b > 0 && a < INT_MIN / b)) {
                throw std::underflow_error("Multiplication would underflow");
            }
        }
        
        int result = a * b;
        std::cout << "[DebugMath] Safe multiplication result: " << result 
                  << " (validated for overflow)" << std::endl;
        return result;
    }
    
    int square(int n) {
        std::cout << "[DebugMath] Computing square of " << n << std::endl;
        std::cout << "[DebugMath] Step-by-step: " << n << " * " << n;
        
        // Overflow check for square
        if (n > 0 && n > INT_MAX / n) {
            throw std::overflow_error("Square would overflow");
        }
        if (n < 0 && n < INT_MAX / n) {
            throw std::overflow_error("Square would overflow");
        }
        
        int result = n * n;
        std::cout << " = " << result << " (validated)" << std::endl;
        return result;
    }
    
    int abs(int n) {
        std::cout << "[DebugMath] Computing absolute value of " << n << std::endl;
        
        if (n == INT_MIN) {
            throw std::overflow_error("abs(INT_MIN) would overflow");
        }
        
        int result = n < 0 ? -n : n;
        std::cout << "[DebugMath] abs(" << n << ") = " << result 
                  << " (safe conversion)" << std::endl;
        return result;
    }
    
    void debug_info(const std::string& operation) {
        std::cout << "[DebugMath] === " << operation << " Debug Info ===" << std::endl;
        std::cout << "[DebugMath] INT_MAX = " << INT_MAX << std::endl;
        std::cout << "[DebugMath] INT_MIN = " << INT_MIN << std::endl;
        std::cout << "[DebugMath] Full bounds checking enabled" << std::endl;
    }
    
    // Registration function
    void register_functions() {
        auto& registry = MultiModuleFunctionRegistry::instance();
        
        registry.register_function("DebugMath", StandardFunctionId::ADD, 
                                 "Addition with overflow protection and logging", add);
        registry.register_function("DebugMath", StandardFunctionId::MULTIPLY, 
                                 "Multiplication with bounds checking", multiply);
        registry.register_function("DebugMath", StandardFunctionId::SQUARE, 
                                 "Square with step-by-step logging", square);
        registry.register_function("DebugMath", StandardFunctionId::ABS, 
                                 "Absolute value with edge case handling", abs);
    }
}

// Static registration
namespace {
    struct DebugMathRegistrar {
        DebugMathRegistrar() {
            DebugMath::register_functions();
        }
    };
    static DebugMathRegistrar debug_math_registrar;
}
