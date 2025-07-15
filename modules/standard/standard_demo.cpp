#include "standard_registry.h"
#include <iostream>
#include <vector>
#include <functional>

using namespace Standard;

int main() {
    std::cout << "=== Standard Function Registration Demo ===" << std::endl;
    
    // Get registry instance and initialize
    auto& registry = StandardRegistry::getInstance();
    registry.initializeStandardFunctions();
    
    std::cout << "Registered " << registry.getFunctionCount() << " functions" << std::endl;
    
    // Test math functions
    std::cout << "\n=== Math Functions ===" << std::endl;
    
    auto* addFunc = registry.getFunction<std::function<int(int, int)>>("add");
    auto* sqrtFunc = registry.getFunction<std::function<double(double)>>("sqrt");
    
    if (addFunc) {
        std::cout << "add(10, 5) = " << (*addFunc)(10, 5) << std::endl;
    }
    
    if (sqrtFunc) {
        std::cout << "sqrt(16) = " << (*sqrtFunc)(16) << std::endl;
    }
    
    // Test string functions
    std::cout << "\n=== String Functions ===" << std::endl;
    
    auto* upperFunc = registry.getFunction<std::function<std::string(const std::string&)>>("to_upper");
    auto* lengthFunc = registry.getFunction<std::function<size_t(const std::string&)>>("length");
    
    if (upperFunc) {
        std::string test = "hello world";
        std::cout << "to_upper('" << test << "') = '" << (*upperFunc)(test) << "'" << std::endl;
    }
    
    if (lengthFunc) {
        std::string test = "standard";
        std::cout << "length('" << test << "') = " << (*lengthFunc)(test) << std::endl;
    }
    
    // Test utility functions
    std::cout << "\n=== Utility Functions ===" << std::endl;
    
    auto* versionFunc = registry.getFunction<std::function<std::string()>>("version");
    auto* printFunc = registry.getFunction<std::function<void(const std::string&)>>("print");
    
    if (versionFunc) {
        std::cout << "Version: " << (*versionFunc)() << std::endl;
    }
    
    if (printFunc) {
        (*printFunc)("This is a test message");
    }
    
    // List all registered functions
    std::cout << "\n=== All Registered Functions ===" << std::endl;
    auto functionNames = registry.getFunctionNames();
    for (const auto& name : functionNames) {
        std::cout << "- " << name << std::endl;
    }
    
    std::cout << "\n=== Standard Demo Complete ===" << std::endl;
    return 0;
}
