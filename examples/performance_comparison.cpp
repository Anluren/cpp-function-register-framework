#include <iostream>
#include <chrono>
#include <vector>
#include "../include/simple_modern_function_group.h"
#include "../include/optimized_function_group.h"

// Example functions that might be used in different scenarios
double expensive_calculation(double x, double y) {
    // Simulate some computation
    double result = 0.0;
    for (int i = 0; i < 1000; ++i) {
        result += std::sin(x + i) * std::cos(y + i);
    }
    return result;
}

int simple_add(int a, int b) {
    return a + b;
}

std::string format_result(double value) {
    return "Result: " + std::to_string(value);
}

// Benchmark helper
template<typename Func>
double measure_execution_time(Func&& func, int iterations = 10000) {
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < iterations; ++i) {
        func();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    return static_cast<double>(duration.count()) / iterations; // Average per call in nanoseconds
}

int main() {
    std::cout << "=== Practical Performance Examples ===\n\n";
    
    // Setup function groups
    SimpleFunctionGroup simple_group("demo");
    OptimizedFunctionGroup optimized_group("demo");
    
    // Register functions
    simple_group.add("expensive_calc", expensive_calculation);
    simple_group.add("simple_add", simple_add);
    simple_group.add("format", format_result);
    
    constexpr auto EXPENSIVE_CALC_ID = FUNC_ID("expensive_calc");
    constexpr auto SIMPLE_ADD_ID = FUNC_ID("simple_add");
    constexpr auto FORMAT_ID = FUNC_ID("format");
    
    optimized_group.add(EXPENSIVE_CALC_ID, "expensive_calc", expensive_calculation);
    optimized_group.add(SIMPLE_ADD_ID, "simple_add", simple_add);
    optimized_group.add(FORMAT_ID, "format", format_result);
    
    std::cout << "1. EXPENSIVE COMPUTATION (where overhead doesn't matter)\n";
    std::cout << "   Function does significant work - lookup overhead is negligible\n\n";
    
    // For expensive operations, lookup overhead is negligible
    auto expensive_string_time = measure_execution_time([&]() {
        simple_group.call_as<double>("expensive_calc", 1.5, 2.5);
    }, 100); // Fewer iterations for expensive function
    
    auto expensive_id_time = measure_execution_time([&]() {
        optimized_group.call_as<double>(EXPENSIVE_CALC_ID, 1.5, 2.5);
    }, 100);
    
    std::cout << "   String lookup:  " << expensive_string_time / 1000 << " μs per call\n";
    std::cout << "   ID lookup:      " << expensive_id_time / 1000 << " μs per call\n";
    std::cout << "   Performance difference: " << 
        std::abs(expensive_string_time - expensive_id_time) / 1000 << " μs (" <<
        (std::abs(expensive_string_time - expensive_id_time) / expensive_string_time) * 100 << "%)\n";
    std::cout << "   → Recommendation: Use readable string-based lookup\n\n";
    
    std::cout << "2. LIGHTWEIGHT COMPUTATION (where overhead matters)\n";
    std::cout << "   Function does minimal work - lookup overhead is significant\n\n";
    
    // For lightweight operations, lookup overhead matters more
    auto light_string_time = measure_execution_time([&]() {
        simple_group.call_as<int>("simple_add", 10, 20);
    });
    
    auto light_id_time = measure_execution_time([&]() {
        optimized_group.call_as<int>(SIMPLE_ADD_ID, 10, 20);
    });
    
    std::cout << "   String lookup:  " << light_string_time << " ns per call\n";
    std::cout << "   ID lookup:      " << light_id_time << " ns per call\n";
    std::cout << "   Performance difference: " << 
        std::abs(light_string_time - light_id_time) << " ns (" <<
        (std::abs(light_string_time - light_id_time) / light_string_time) * 100 << "%)\n";
    std::cout << "   → Recommendation: Consider ID-based lookup for high-frequency calls\n\n";
    
    std::cout << "3. REAL-WORLD SCENARIOS\n\n";
    
    // Scenario 1: Configuration-driven calls (development/debugging)
    std::cout << "   Scenario A: Configuration-driven function calls\n";
    std::vector<std::string> config_functions = {"simple_add", "format"};
    
    for (const auto& func_name : config_functions) {
        if (func_name == "simple_add") {
            auto result = simple_group.call_as<int>(func_name, 5, 3);
            std::cout << "      " << func_name << "(5, 3) = " << result << "\n";
        } else if (func_name == "format") {
            auto result = simple_group.call_as<std::string>(func_name, 42.5);
            std::cout << "      " << func_name << "(42.5) = " << result << "\n";
        }
    }
    std::cout << "   → String-based lookup is perfect for this use case\n\n";
    
    // Scenario 2: High-frequency game loop
    std::cout << "   Scenario B: High-frequency game loop (60 FPS)\n";
    std::cout << "   Calling simple_add 1000 times per frame...\n";
    
    constexpr int CALLS_PER_FRAME = 1000;
    constexpr int TARGET_FPS = 60;
    constexpr double FRAME_BUDGET_MS = 1000.0 / TARGET_FPS; // ~16.67ms per frame
    
    auto frame_string_time = measure_execution_time([&]() {
        for (int i = 0; i < CALLS_PER_FRAME; ++i) {
            simple_group.call_as<int>("simple_add", i, i + 1);
        }
    }, 10); // 10 frames
    
    auto frame_id_time = measure_execution_time([&]() {
        for (int i = 0; i < CALLS_PER_FRAME; ++i) {
            optimized_group.call_as<int>(SIMPLE_ADD_ID, i, i + 1);
        }
    }, 10); // 10 frames
    
    double frame_string_ms = frame_string_time / 1000000.0; // Convert to milliseconds
    double frame_id_ms = frame_id_time / 1000000.0;
    
    std::cout << "      String lookup:  " << frame_string_ms << " ms per frame (" <<
        (frame_string_ms / FRAME_BUDGET_MS) * 100 << "% of frame budget)\n";
    std::cout << "      ID lookup:      " << frame_id_ms << " ms per frame (" <<
        (frame_id_ms / FRAME_BUDGET_MS) * 100 << "% of frame budget)\n";
    std::cout << "      Saved time:     " << frame_string_ms - frame_id_ms << " ms per frame\n";
    
    if (frame_string_ms > 1.0) { // If string lookup takes more than 1ms per frame
        std::cout << "   → ID-based lookup recommended for performance\n\n";
    } else {
        std::cout << "   → Either approach is acceptable\n\n";
    }
    
    std::cout << "4. MIGRATION STRATEGY\n\n";
    std::cout << "   Step 1: Start with string-based lookup for development\n";
    std::cout << "   Step 2: Profile to identify hot paths\n";
    std::cout << "   Step 3: Migrate hot paths to ID-based lookup\n";
    std::cout << "   Step 4: Keep string lookup for debugging/config\n\n";
    
    std::cout << "   Example migration:\n";
    std::cout << "   // Development phase\n";
    std::cout << "   group.call_as<int>(\"simple_add\", a, b);  // Readable\n\n";
    std::cout << "   // Production phase (after profiling shows this is hot)\n";
    std::cout << "   constexpr auto ADD_ID = FUNC_ID(\"simple_add\");\n";
    std::cout << "   group.call_as<int>(ADD_ID, a, b);  // Optimized\n\n";
    
    std::cout << "=== Conclusion ===\n";
    std::cout << "Performance optimization should be:\n";
    std::cout << "1. Measured, not assumed\n";
    std::cout << "2. Applied where it actually matters\n";
    std::cout << "3. Balanced with code readability and maintainability\n";
    std::cout << "\nThe framework provides flexibility to optimize when and where needed!\n";
    
    return 0;
}
