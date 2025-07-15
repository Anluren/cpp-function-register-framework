#include "legacy_registry.h"
#include "legacy_init.h"
#include <iostream>

using namespace Legacy;

int main() {
    std::cout << "=== Legacy Function Registration Demo ===" << std::endl;
    
    // Initialize all legacy modules
    initializeLegacyModules();
    
    // Get registry instance
    LegacyRegistry& registry = LegacyRegistry::getInstance();
    
    // List available groups
    std::cout << "\nAvailable function groups:" << std::endl;
    std::vector<FunctionType> groupTypes = registry.getGroupTypes();
    for (size_t i = 0; i < groupTypes.size(); ++i) {
        FunctionType type = groupTypes[i];
        LegacyFunctionGroup* group = registry.getGroup(type);
        if (group) {
            std::cout << "- " << group->getName() << " (" << group->getFunctionCount() << " functions)" << std::endl;
        }
    }
    
    // Test math functions
    std::cout << "\n=== Math Functions ===" << std::endl;
    IntIntIntFunction addFunc = LEGACY_GET_FUNCTION(MATH, "add", IntIntIntFunction);
    IntIntIntFunction subtractFunc = LEGACY_GET_FUNCTION(MATH, "subtract", IntIntIntFunction);
    DoubleDoubleFunction sqrtFunc = LEGACY_GET_FUNCTION(MATH, "sqrt", DoubleDoubleFunction);
    
    if (addFunc && subtractFunc && sqrtFunc) {
        std::cout << "add(10, 5) = " << addFunc(10, 5) << std::endl;
        std::cout << "subtract(10, 5) = " << subtractFunc(10, 5) << std::endl;
        std::cout << "sqrt(16.0) = " << sqrtFunc(16.0) << std::endl;
    } else {
        std::cout << "Math functions not found!" << std::endl;
    }
    
    // Test string functions
    std::cout << "\n=== String Functions ===" << std::endl;
    StringStringFunction upperFunc = LEGACY_GET_FUNCTION(STRING, "to_upper", StringStringFunction);
    StringStringFunction lowerFunc = LEGACY_GET_FUNCTION(STRING, "to_lower", StringStringFunction);
    typedef int (*StringLengthFunction)(const char*);
    StringLengthFunction lengthFunc = LEGACY_GET_FUNCTION(STRING, "length", StringLengthFunction);
    
    if (upperFunc && lowerFunc && lengthFunc) {
        const char* testStr = "Hello World";
        std::cout << "Original: " << testStr << std::endl;
        std::cout << "Upper: " << upperFunc(testStr) << std::endl;
        std::cout << "Lower: " << lowerFunc(testStr) << std::endl;
        std::cout << "Length: " << lengthFunc(testStr) << std::endl;
    } else {
        std::cout << "String functions not found!" << std::endl;
    }
    
    // Test utility functions
    std::cout << "\n=== Utility Functions ===" << std::endl;
    StringFunction versionFunc = LEGACY_GET_FUNCTION(UTILITY, "version", StringFunction);
    IntIntFunction factorialFunc = LEGACY_GET_FUNCTION(UTILITY, "factorial", IntIntFunction);
    IntFunction randomFunc = LEGACY_GET_FUNCTION(UTILITY, "random", IntFunction);
    
    if (versionFunc && factorialFunc && randomFunc) {
        std::cout << "Version: " << versionFunc() << std::endl;
        std::cout << "factorial(5) = " << factorialFunc(5) << std::endl;
        std::cout << "Random number: " << randomFunc() << std::endl;
    } else {
        std::cout << "Utility functions not found!" << std::endl;
    }
    
    // List all functions in each group
    std::cout << "\n=== Function Lists ===" << std::endl;
    for (size_t i = 0; i < groupTypes.size(); ++i) {
        FunctionType type = groupTypes[i];
        LegacyFunctionGroup* group = registry.getGroup(type);
        if (group) {
            std::cout << "\n" << group->getName() << ":" << std::endl;
            std::vector<std::string> functionNames = group->getFunctionNames();
            for (size_t j = 0; j < functionNames.size(); ++j) {
                std::cout << "  - " << functionNames[j] << std::endl;
            }
        }
    }
    
    std::cout << "\n=== Legacy Demo Complete ===" << std::endl;
    return 0;
}
