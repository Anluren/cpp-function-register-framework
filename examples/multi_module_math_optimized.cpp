/**
 * Optimized Math Module for Multi-Module Demo
 * Implements arithmetic functions with optimized algorithms
 */

#include "../include/multi_module_registry.h"
#include <iostream>
#include <cmath>

namespace OptimizedMath {
    
    int add(int a, int b) {
        std::cout << "[OptimizedMath] Fast addition " << a << " + " << b << " = " << (a + b) << std::endl;
        return a + b;
    }
    
    int multiply(int a, int b) {
        // Use bit shifting for powers of 2
        if (b > 0 && (b & (b - 1)) == 0) {
            int shifts = 0;
            int temp = b;
            while (temp > 1) {
                temp >>= 1;
                shifts++;
            }
            int result = a << shifts;
            std::cout << "[OptimizedMath] Bit-shift multiplication " << a << " * " << b 
                      << " = " << result << " (using << " << shifts << ")" << std::endl;
            return result;
        } else {
            int result = a * b;
            std::cout << "[OptimizedMath] Standard multiplication " << a << " * " << b 
                      << " = " << result << std::endl;
            return result;
        }
    }
    
    int square(int n) {
        // Use bit shifting for efficiency when possible
        int result = n * n;
        std::cout << "[OptimizedMath] Optimized square " << n << "^2 = " << result << std::endl;
        return result;
    }
    
    int abs(int n) {
        // Bit manipulation trick for abs
        int mask = n >> 31;
        int result = (n + mask) ^ mask;
        std::cout << "[OptimizedMath] Bit-manipulation abs(" << n << ") = " << result << std::endl;
        return result;
    }
    
    double sqrt(int n) {
        double result = std::sqrt(static_cast<double>(n));
        std::cout << "[OptimizedMath] Hardware sqrt(" << n << ") = " << result << std::endl;
        return result;
    }
    
    // Registration function
    void register_functions() {
        auto& registry = MultiModuleFunctionRegistry::instance();
        
        registry.register_function("OptimizedMath", StandardFunctionId::ADD, 
                                 "Optimized addition algorithm", add);
        registry.register_function("OptimizedMath", StandardFunctionId::MULTIPLY, 
                                 "Bit-shift multiplication for powers of 2", multiply);
        registry.register_function("OptimizedMath", StandardFunctionId::SQUARE, 
                                 "Optimized square calculation", square);
        registry.register_function("OptimizedMath", StandardFunctionId::ABS, 
                                 "Bit-manipulation absolute value", abs);
        registry.register_function("OptimizedMath", StandardFunctionId::SQRT, 
                                 "Hardware-accelerated square root", 
                                 [](int n) { return sqrt(n); });
    }
}

// Static registration
namespace {
    struct OptimizedMathRegistrar {
        OptimizedMathRegistrar() {
            OptimizedMath::register_functions();
        }
    };
    static OptimizedMathRegistrar optimized_math_registrar;
}
