#include "optimized_registry.h"
#include <iostream>
#include <chrono>

using namespace Optimized;

    // void initializeOptimizedModule();
int main() {
    std::cout << "=== Optimized Function Registration Demo ===" << std::endl;
    
    // Initialize the optimized module
    initializeOptimizedModule();
    
    // Get registry instance
    auto& registry = OptimizedRegistry::getInstance();
    
    // List available categories
    std::cout << "\nAvailable function categories:" << std::endl;
    auto categories = registry.getCategories();
    std::cout << "Found " << categories.size() << " categories" << std::endl;
    
    for (auto category : categories) {
        auto* group = registry.getGroup(category);
        if (group) {
            std::cout << "- " << group->getName() << " (" << group->getFunctionCount() << " functions)" << std::endl;
        }
    }
    
    // Test math functions
    std::cout << "\n=== Math Functions ===" << std::endl;
    
    auto addFunc = registry.getFunction<int, int, int>(FunctionCategory::MATH, "add");
    auto factorialFunc = registry.getFunction<int, int>(FunctionCategory::MATH, "factorial");
    
    if (addFunc && factorialFunc) {
        std::cout << "add(10, 5) = " << addFunc(10, 5) << std::endl;
        std::cout << "factorial(8) = " << factorialFunc(8) << std::endl;
    } else {
        std::cout << "Math functions not found!" << std::endl;
    }
    
    std::cout << "\n=== Optimized Demo Complete ===" << std::endl;
    return 0;
}
