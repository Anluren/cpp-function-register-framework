#include "function_registry.h"
#include <iostream>

// Simple example functions
void* hello_world() {
    std::cout << "Hello, World!" << std::endl;
    static bool success = true;
    return &success;
}

void* add_ten(void* input) {
    int value = *static_cast<int*>(input);
    static int result = value + 10;
    return &result;
}

// Initialize simple functions
void initSimpleFunctions() {
    ConcreteFunctionGroup* simpleGroup = new ConcreteFunctionGroup("Simple Functions");
    
    simpleGroup->registerFunction0("hello", hello_world);
    simpleGroup->registerFunction1("add_ten", add_ten);
    
    FunctionRegistry::getInstance().registerGroup(CUSTOM_GROUP_1, simpleGroup);
}

// Auto-register
static class SimpleFunctionInitializer {
public:
    SimpleFunctionInitializer() {
        initSimpleFunctions();
    }
} simpleInit;

int main() {
    std::cout << "=== Simple Function Registration Example ===" << std::endl;
    
    FunctionRegistry& registry = FunctionRegistry::getInstance();
    FunctionGroup* group = registry.getGroup(CUSTOM_GROUP_1);
    
    if (group) {
        std::cout << "Found group: " << group->getName() << std::endl;
        
        // Call hello function
        if (group->hasFunction("hello")) {
            group->callFunction("hello");
        }
        
        // Call add_ten function
        if (group->hasFunction("add_ten")) {
            int input = 5;
            void* result = group->callFunction("add_ten", &input);
            int output = *static_cast<int*>(result);
            std::cout << "5 + 10 = " << output << std::endl;
        }
    } else {
        std::cout << "No function group found!" << std::endl;
    }
    
    return 0;
}
