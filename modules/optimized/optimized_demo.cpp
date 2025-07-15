#include "optimized_registry.h"
#include <iostream>
#include <chrono>

using namespace Optimized;

int main() {
    std::cout << "=== Optimized Function Registration Demo ===" << std::endl;
    
    // Get registry instance
    auto& registry = OptimizedRegistry::getInstance();
    
    // List available categories
    std::cout << "\nAvailable function categories:" << std::endl;
    auto categories = registry.getCategories();
    for (auto category : categories) {
        auto* group = registry.getGroup(category);
        if (group) {
            std::cout << "- " << group->getName() << " (" << group->getFunctionCount() << " functions)" << std::endl;
        }
    }
    
    // Performance test - Math Functions
    std::cout << "\n=== Math Functions Performance Test ===" << std::endl;
    
    auto addFunc = registry.getFunction<int, int, int>(FunctionCategory::MATH, "add");
    auto factorialFunc = registry.getFunction<int, int>(FunctionCategory::MATH, "factorial");
    
    if (addFunc && factorialFunc) {
        // Time the function calls
        auto start = std::chrono::high_resolution_clock::now();
        
        int sum = 0;
        for (int i = 0; i < 1000000; ++i) {
            sum += addFunc(i, i + 1);
        }
        
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        
        std::cout << "add(10, 5) = " << addFunc(10, 5) << std::endl;
        std::cout << "factorial(8) = " << factorialFunc(8) << std::endl;
        std::cout << "Performance: 1M additions took " << duration.count() << " microseconds" << std::endl;
    }
    
    // Test string functions
    std::cout << "\n=== String Functions ===" << std::endl;
    
    auto upperFunc = registry.getFunction<const char*, const char*>(FunctionCategory::STRING, "to_upper");
    auto lengthFunc = registry.getFunction<int, const char*>(FunctionCategory::STRING, "length");
    
    if (upperFunc && lengthFunc) {
        const char* testStr = "Hello Optimized World";
        std::cout << "Original: " << testStr << std::endl;
        std::cout << "Upper: " << upperFunc(testStr) << std::endl;
        std::cout << "Length: " << lengthFunc(testStr) << std::endl;
    }
    
    // Test utility functions
    std::cout << "\n=== Utility Functions ===" << std::endl;
    
    auto versionFunc = registry.getFunction<const char*>(FunctionCategory::UTILITY, "version");
    auto timestampFunc = registry.getFunction<long long>(FunctionCategory::UTILITY, "timestamp");
    auto randomRangeFunc = registry.getFunction<int, int, int>(FunctionCategory::UTILITY, "random_range");
    
    if (versionFunc && timestampFunc && randomRangeFunc) {
        std::cout << "Version: " << versionFunc() << std::endl;
        std::cout << "Timestamp: " << timestampFunc() << std::endl;
        std::cout << "Random (1-100): " << randomRangeFunc(1, 100) << std::endl;
    }
    
    // Direct function call performance comparison
    std::cout << "\n=== Direct vs Registry Call Performance ===" << std::endl;
    
    if (addFunc) {
        // Registry call performance
        auto start1 = std::chrono::high_resolution_clock::now();
        
        int sum1 = 0;
        for (int i = 0; i < 1000000; ++i) {
            sum1 += addFunc(i, i + 1);
        }
        
        auto end1 = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - start1);
        
        std::cout << "Direct function pointer: 1M additions took " << duration1.count() << " microseconds" << std::endl;
        std::cout << "Sum result: " << sum1 << std::endl;
    }
    
    // Test high-performance features
    std::cout << "\n=== High-Performance Features ===" << std::endl;
    
    // Test hash function lookup
    auto* mathGroup = registry.getGroup(FunctionCategory::MATH);
    if (mathGroup) {
        std::cout << "Math group lookup time is O(1) with hash-based function storage" << std::endl;
        std::cout << "Function exists check: " << std::boolalpha << mathGroup->hasFunction("add") << std::endl;
    }
    
    // List all functions in each category
    std::cout << "\n=== Function Lists ===" << std::endl;
    for (auto category : categories) {
        auto* group = registry.getGroup(category);
        if (group) {
            std::cout << "\n" << group->getName() << ":" << std::endl;
            auto functionNames = group->getFunctionNames();
            for (const auto& name : functionNames) {
                std::cout << "  - " << name << std::endl;
            }
        }
    }
    
    std::cout << "\n=== Optimized Demo Complete ===" << std::endl;
    return 0;
}
