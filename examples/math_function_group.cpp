#include "simple_modern_function_group.h"

// Math function implementations
int add(int a, int b) { 
    return a + b; 
}

double multiply(double a, double b) { 
    return a * b; 
}

int get_random() { 
    return 42; 
}

double power(double base, double exponent) {
    double result = 1.0;
    for (int i = 0; i < static_cast<int>(exponent); ++i) {
        result *= base;
    }
    return result;
}

// Function to register math functions
void register_math_functions() {
    auto math_group = std::make_unique<SimpleFunctionGroup>("Math Operations");
    
    // Register function pointers
    math_group->add("add", add);
    math_group->add("multiply", multiply);
    math_group->add("random", get_random);
    math_group->add("power", power);
    
    // Register lambdas for additional math operations
    math_group->add("square", [](int x) { return x * x; });
    math_group->add("cube", [](int x) { return x * x * x; });
    math_group->add("factorial", [](int n) {
        int result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    });
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::MATH_FUNCTIONS, std::move(math_group));
}
