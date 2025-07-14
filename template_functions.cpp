/*
 * Template for creating new function groups
 * 
 * Instructions:
 * 1. Copy this file and rename it (e.g., my_functions.cpp)
 * 2. Replace TEMPLATE_FUNCTIONS with your enum value from function_group.h
 * 3. Replace "Template Functions" with your group name
 * 4. Implement your functions following the examples below
 * 5. Register your functions in the initTemplateFunctions() function
 * 6. Add your .cpp file to the Makefile SOURCES
 * 7. Build and run!
 */

#include "function_registry.h"

// Example function with no parameters
void* template_func_0() {
    static int result = 42;  // Use static for persistent storage
    return &result;
}

// Example function with one parameter
void* template_func_1(void* param) {
    // Cast the parameter to the appropriate type
    int input = *static_cast<int*>(param);
    static int result = input * 2;
    return &result;
}

// Example function with two parameters  
void* template_func_2(void* param1, void* param2) {
    // Cast both parameters
    int a = *static_cast<int*>(param1);
    int b = *static_cast<int*>(param2);
    static int result = a + b;
    return &result;
}

// Initialize and register your function group
void initTemplateFunctions() {
    // Create a new function group
    ConcreteFunctionGroup* templateGroup = new ConcreteFunctionGroup("Template Functions");
    
    // Register your functions
    templateGroup->registerFunction0("func0", template_func_0);
    templateGroup->registerFunction1("func1", template_func_1);
    templateGroup->registerFunction2("func2", template_func_2);
    
    // Register the group with the registry
    // IMPORTANT: Change TEMPLATE_FUNCTIONS to your enum value!
    // FunctionRegistry::getInstance().registerGroup(YOUR_ENUM_VALUE, templateGroup);
}

// Automatic registration using static initialization
// This ensures your functions are registered when the program starts
static class TemplateFunctionInitializer {
public:
    TemplateFunctionInitializer() {
        initTemplateFunctions();
    }
} templateInit;

/*
 * Usage Example:
 * 
 * // In your main code:
 * FunctionRegistry& registry = FunctionRegistry::getInstance();
 * FunctionGroup* group = registry.getGroup(YOUR_ENUM_VALUE);
 * 
 * if (group && group->hasFunction("func1")) {
 *     int input = 10;
 *     void* result = group->callFunction("func1", &input);
 *     int output = *static_cast<int*>(result);
 *     std::cout << "Result: " << output << std::endl;
 * }
 */
