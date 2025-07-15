#include "modern_registry.h"
#include <iostream>
#include <iomanip>
#include <future>

using namespace Modern;

int main() {
    std::cout << "=== Modern Function Registration Demo ===" << std::endl;
    
    // Get registry instance and initialize modules
    auto& registry = ModernRegistry::getInstance();
    registry.initializeModules();
    
    // List available groups
    std::cout << "\nAvailable function categories:" << std::endl;
    auto categories = registry.getCategories();
    for (auto category : categories) {
        auto* group = registry.getGroup(category);
        if (group) {
            std::cout << "- " << group->getName() << " (" << group->getFunctionCount() << " functions)" << std::endl;
        }
    }
    
    // Test math functions
    std::cout << "\n=== Math Functions ===" << std::endl;
    
    // Basic arithmetic
    auto addFunc = MODERN_GET_FUNCTION(FunctionCategory::MATH, "add", int(int, int));
    auto divideFunc = MODERN_GET_FUNCTION(FunctionCategory::MATH, "divide", double(int, int));
    
    if (addFunc && divideFunc) {
        std::cout << "add(10, 5) = " << addFunc->call(10, 5) << std::endl;
        std::cout << "divide(10, 3) = " << std::fixed << std::setprecision(2) 
                  << divideFunc->call(10, 3) << std::endl;
    }
    
    // Advanced math
    auto factorialFunc = MODERN_GET_FUNCTION(FunctionCategory::MATH, "factorial", int(int));
    auto fibonacciFunc = MODERN_GET_FUNCTION(FunctionCategory::MATH, "fibonacci", long long(int));
    
    if (factorialFunc && fibonacciFunc) {
        std::cout << "factorial(6) = " << factorialFunc->call(6) << std::endl;
        std::cout << "fibonacci(10) = " << fibonacciFunc->call(10) << std::endl;
    }
    
    // Vector operations
    auto vectorSumFunc = MODERN_GET_FUNCTION(FunctionCategory::MATH, "vector_sum", int(const std::vector<int>&));
    
    if (vectorSumFunc) {
        std::vector<int> numbers = {1, 2, 3, 4, 5};
        std::cout << "vector_sum([1,2,3,4,5]) = " << vectorSumFunc->call(numbers) << std::endl;
    }
    
    // Test string functions
    std::cout << "\n=== String Functions ===" << std::endl;
    
    auto upperFunc = MODERN_GET_FUNCTION(FunctionCategory::STRING, "to_upper", std::string(const std::string&));
    auto reverseFunc = MODERN_GET_FUNCTION(FunctionCategory::STRING, "reverse", std::string(const std::string&));
    auto trimFunc = MODERN_GET_FUNCTION(FunctionCategory::STRING, "trim", std::string(const std::string&));
    
    if (upperFunc && reverseFunc && trimFunc) {
        std::string testStr = "  Hello Modern World  ";
        std::cout << "Original: '" << testStr << "'" << std::endl;
        std::cout << "Upper: '" << upperFunc->call(testStr) << "'" << std::endl;
        std::cout << "Reverse: '" << reverseFunc->call(testStr) << "'" << std::endl;
        std::cout << "Trim: '" << trimFunc->call(testStr) << "'" << std::endl;
    }
    
    // Advanced string operations
    auto splitFunc = MODERN_GET_FUNCTION(FunctionCategory::STRING, "split", std::vector<std::string>(const std::string&, char));
    auto isEmailFunc = MODERN_GET_FUNCTION(FunctionCategory::STRING, "is_email", bool(const std::string&));
    
    if (splitFunc && isEmailFunc) {
        std::string sentence = "apple,banana,cherry";
        auto words = splitFunc->call(sentence, ',');
        std::cout << "split('" << sentence << "', ',') = [";
        for (size_t i = 0; i < words.size(); ++i) {
            std::cout << "'" << words[i] << "'";
            if (i < words.size() - 1) std::cout << ", ";
        }
        std::cout << "]" << std::endl;
        
        std::string email = "test@example.com";
        std::cout << "is_email('" << email << "') = " << std::boolalpha 
                  << isEmailFunc->call(email) << std::endl;
    }
    
    // Test utility functions
    std::cout << "\n=== Utility Functions ===" << std::endl;
    
    auto versionFunc = MODERN_GET_FUNCTION(FunctionCategory::UTILITY, "version", std::string());
    auto timestampFunc = MODERN_GET_FUNCTION(FunctionCategory::UTILITY, "current_timestamp", long long());
    auto randomIntFunc = MODERN_GET_FUNCTION(FunctionCategory::UTILITY, "random_int", int(int, int));
    auto cpuCountFunc = MODERN_GET_FUNCTION(FunctionCategory::UTILITY, "cpu_count", unsigned int());
    
    if (versionFunc && timestampFunc && randomIntFunc && cpuCountFunc) {
        std::cout << "Version: " << versionFunc->call() << std::endl;
        std::cout << "Current timestamp: " << timestampFunc->call() << std::endl;
        std::cout << "Random number (1-100): " << randomIntFunc->call(1, 100) << std::endl;
        std::cout << "CPU cores: " << cpuCountFunc->call() << std::endl;
    }
    
    // Test async function (note: async returns std::future which is complex to handle in demo)
    std::cout << "Async factorial function available but skipped in demo due to std::future complexity" << std::endl;
    
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
    
    std::cout << "\n=== Modern Demo Complete ===" << std::endl;
    return 0;
}
