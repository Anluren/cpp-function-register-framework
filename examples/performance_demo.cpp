#include "simple_modern_function_group.h"
#include "optimized_function_group.h"
#include <iostream>
#include <chrono>
#include <string>

// Test functions
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }
int subtract(int a, int b) { return a - b; }

// Performance testing utilities
template<typename Func>
void benchmark(const std::string& name, Func&& func, int iterations = 100000) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    
    std::cout << name << ": " << duration.count() << " microseconds (" 
              << (duration.count() / static_cast<double>(iterations)) << " µs per call)" << std::endl;
}

int main() {
    std::cout << "=== Function Registration Performance Comparison ===" << std::endl;
    
    const int ITERATIONS = 100000;
    
    // Setup regular function group
    auto regular_group = std::make_unique<SimpleFunctionGroup>("Math");
    regular_group->add("add", add);
    regular_group->add("multiply", multiply);
    regular_group->add("subtract", subtract);
    
    // Setup optimized function group
    auto optimized_group = std::make_unique<OptimizedFunctionGroup>("Math Optimized");
    
    // Register with both string names and compile-time IDs
    constexpr FunctionId ADD_ID = FUNC_ID("add");
    constexpr FunctionId MUL_ID = FUNC_ID("multiply");
    constexpr FunctionId SUB_ID = FUNC_ID("subtract");
    
    optimized_group->add(ADD_ID, "add", add);
    optimized_group->add(MUL_ID, "multiply", multiply);
    optimized_group->add(SUB_ID, "subtract", subtract);
    
    std::cout << "\\nTesting with " << ITERATIONS << " function calls each..." << std::endl;
    
    // Benchmark regular string-based lookup
    std::cout << "\\n=== Regular Implementation (std::map<std::string>) ===" << std::endl;
    
    benchmark("String lookup - add", [&]() {
        regular_group->call_as<int>("add", 10, 20);
    }, ITERATIONS);
    
    benchmark("String lookup - multiply", [&]() {
        regular_group->call_as<int>("multiply", 5, 4);
    }, ITERATIONS);
    
    benchmark("String lookup - subtract", [&]() {
        regular_group->call_as<int>("subtract", 30, 10);
    }, ITERATIONS);
    
    // Benchmark optimized ID-based lookup
    std::cout << "\\n=== Optimized Implementation (std::unordered_map<FunctionId>) ===" << std::endl;
    
    benchmark("ID lookup - add", [&]() {
        optimized_group->call_as<int>(ADD_ID, 10, 20);
    }, ITERATIONS);
    
    benchmark("ID lookup - multiply", [&]() {
        optimized_group->call_as<int>(MUL_ID, 5, 4);
    }, ITERATIONS);
    
    benchmark("ID lookup - subtract", [&]() {
        optimized_group->call_as<int>(SUB_ID, 30, 10);
    }, ITERATIONS);
    
    // Benchmark optimized but still using string lookup (for compatibility)
    std::cout << "\\n=== Optimized Implementation with String Lookup (backward compatibility) ===" << std::endl;
    
    benchmark("Optimized string lookup - add", [&]() {
        optimized_group->call_as<int>("add", 10, 20);
    }, ITERATIONS);
    
    benchmark("Optimized string lookup - multiply", [&]() {
        optimized_group->call_as<int>("multiply", 5, 4);
    }, ITERATIONS);
    
    benchmark("Optimized string lookup - subtract", [&]() {
        optimized_group->call_as<int>("subtract", 30, 10);
    }, ITERATIONS);
    
    // Verify results are correct
    std::cout << "\\n=== Correctness Verification ===" << std::endl;
    std::cout << "Regular add(10, 20) = " << regular_group->call_as<int>("add", 10, 20) << std::endl;
    std::cout << "Optimized add(10, 20) = " << optimized_group->call_as<int>(ADD_ID, 10, 20) << std::endl;
    std::cout << "Optimized string add(10, 20) = " << optimized_group->call_as<int>("add", 10, 20) << std::endl;
    
    // Memory usage comparison
    std::cout << "\\n=== Memory and Complexity Analysis ===" << std::endl;
    std::cout << "Regular Implementation:" << std::endl;
    std::cout << "  - Function lookup: O(log n) with std::map<std::string>" << std::endl;
    std::cout << "  - String comparison overhead on each lookup" << std::endl;
    std::cout << "  - Memory: strings stored as keys" << std::endl;
    
    std::cout << "\\nOptimized Implementation:" << std::endl;
    std::cout << "  - Function lookup: O(1) average with std::unordered_map<FunctionId>" << std::endl;
    std::cout << "  - Compile-time ID generation eliminates runtime string processing" << std::endl;
    std::cout << "  - Memory: integers as keys + string mapping for compatibility" << std::endl;
    
    std::cout << "\\n=== Recommendations ===" << std::endl;
    std::cout << "For performance-critical code:" << std::endl;
    std::cout << "  1. Use compile-time function IDs: FUNC_ID(\\\"function_name\\\")" << std::endl;
    std::cout << "  2. Call with ID: group->call_as<ReturnType>(FUNC_ID(\\\"add\\\"), args...)" << std::endl;
    std::cout << "  3. ~10-50% performance improvement expected" << std::endl;
    
    std::cout << "\\nFor development/debugging:" << std::endl;
    std::cout << "  1. Use string names for readability" << std::endl;
    std::cout << "  2. Convert to IDs later for production builds" << std::endl;
    std::cout << "  3. Both APIs are fully compatible" << std::endl;
    
    return 0;
}
