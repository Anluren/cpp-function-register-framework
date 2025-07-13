#include "function_registry.h"
#include <cmath>
#include <iostream>

// Example math functions that return void* (to be cast to appropriate types)
void* add_func() {
    static double result = 5.0 + 3.0;
    return &result;
}

void* multiply_func(void* args) {
    // Args should point to an array of two doubles
    double* values = static_cast<double*>(args);
    static double result = values[0] * values[1];
    return &result;
}

void* power_func(void* base_ptr, void* exp_ptr) {
    double base = *static_cast<double*>(base_ptr);
    double exp = *static_cast<double*>(exp_ptr);
    static double result = pow(base, exp);
    return &result;
}

void* sqrt_func(void* value_ptr) {
    double value = *static_cast<double*>(value_ptr);
    static double result = sqrt(value);
    return &result;
}

// Create and register the math function group
void initMathFunctions() {
    ConcreteFunctionGroup* mathGroup = new ConcreteFunctionGroup("Math Functions");
    
    // Register functions
    mathGroup->registerFunction0("add", add_func);
    mathGroup->registerFunction1("sqrt", sqrt_func);
    mathGroup->registerFunction1("multiply", multiply_func);  // Using 1 param version with array
    mathGroup->registerFunction2("power", power_func);
    
    // Register the group
    FunctionRegistry::getInstance().registerGroup(MATH_FUNCTIONS, mathGroup);
}

// Use static initialization to register automatically
static class MathFunctionInitializer {
public:
    MathFunctionInitializer() {
        initMathFunctions();
    }
} mathInit;
