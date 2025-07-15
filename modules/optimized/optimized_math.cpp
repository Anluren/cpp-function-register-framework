#include "optimized_registry.h"
#include <cmath>

namespace Optimized {

// High-performance math functions
int add_fast(int a, int b) {
    return a + b;
}

int subtract_fast(int a, int b) {
    return a - b;
}

int multiply_fast(int a, int b) {
    return a * b;
}

int divide_fast(int a, int b) {
    return (b != 0) ? a / b : 0;
}

double sqrt_fast(double x) {
    return std::sqrt(x);
}

double power_fast(double base, double exp) {
    return std::pow(base, exp);
}

// Fast factorial using iterative approach
int factorial_fast(int n) {
    if (n <= 1) return 1;
    int result = 1;
    for (int i = 2; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Auto-registration class
class OptimizedMathRegistrar {
public:
    OptimizedMathRegistrar() {
        // Register the math group
        OPTIMIZED_REGISTER_GROUP(FunctionCategory::MATH, "Optimized Math Functions");
        
        // Register individual functions
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::MATH, "add", add_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::MATH, "subtract", subtract_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::MATH, "multiply", multiply_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::MATH, "divide", divide_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::MATH, "sqrt", sqrt_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::MATH, "power", power_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::MATH, "factorial", factorial_fast);
    }
};

// Static instance to trigger registration
static OptimizedMathRegistrar mathRegistrar;

// Explicit initialization function for static linking
void initMathFunctions() {
    static bool initialized = false;
    if (!initialized) {
        // Force the static registrar to be instantiated
        static OptimizedMathRegistrar registrar;
        initialized = true;
    }
}
} // namespace Optimized
