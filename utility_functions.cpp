#include "function_registry.h"
#include <cstdlib>
#include <ctime>

// Utility functions examples
void* random_func() {
    static int result = rand() % 100;
    return &result;
}

void* max_func(void* a_ptr, void* b_ptr) {
    int a = *static_cast<int*>(a_ptr);
    int b = *static_cast<int*>(b_ptr);
    static int result = (a > b) ? a : b;
    return &result;
}

void* factorial_func(void* n_ptr) {
    int n = *static_cast<int*>(n_ptr);
    static int result = 1;
    result = 1;
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return &result;
}

// Initialize random seed and register utility functions
void initUtilityFunctions() {
    srand(static_cast<unsigned int>(time(0)));
    
    ConcreteFunctionGroup* utilityGroup = new ConcreteFunctionGroup("Utility Functions");
    
    // Register functions
    utilityGroup->registerFunction0("random", random_func);
    utilityGroup->registerFunction1("factorial", factorial_func);
    utilityGroup->registerFunction2("max", max_func);
    
    // Register the group
    FunctionRegistry::getInstance().registerGroup(UTILITY_FUNCTIONS, utilityGroup);
}

// Use static initialization to register automatically
static class UtilityFunctionInitializer {
public:
    UtilityFunctionInitializer() {
        initUtilityFunctions();
    }
} utilityInit;
