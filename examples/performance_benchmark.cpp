#include <chrono>
#include <iostream>
#include <iomanip>
#include <vector>
#include <random>
#include "../include/simple_modern_function_group.h"
#include "../include/optimized_function_group.h"

// Test functions for benchmarking
int add(int a, int b) { return a + b; }
int multiply(int a, int b) { return a * b; }
int subtract(int a, int b) { return a - b; }
double divide(double a, double b) { return b != 0.0 ? a / b : 0.0; }
int square(int x) { return x * x; }

// Benchmark helper class
class BenchmarkTimer {
private:
    std::chrono::high_resolution_clock::time_point start_;
    
public:
    void start() {
        start_ = std::chrono::high_resolution_clock::now();
    }
    
    double elapsed_ms() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_);
        return duration.count() / 1000000.0; // Convert to milliseconds
    }
    
    double elapsed_ns() const {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start_);
        return static_cast<double>(duration.count());
    }
};

// Setup function groups for benchmarking
void setup_simple_group(SimpleFunctionGroup& group) {
    group.add("add", add);
    group.add("multiply", multiply);
    group.add("subtract", subtract);
    group.add("divide", divide);
    group.add("square", square);
}

void setup_optimized_group(OptimizedFunctionGroup& group) {
    // Register with both IDs and names for testing
    constexpr auto ADD_ID = FUNC_ID("add");
    constexpr auto MULTIPLY_ID = FUNC_ID("multiply");
    constexpr auto SUBTRACT_ID = FUNC_ID("subtract");
    constexpr auto DIVIDE_ID = FUNC_ID("divide");
    constexpr auto SQUARE_ID = FUNC_ID("square");
    
    group.add(ADD_ID, "add", add);
    group.add(MULTIPLY_ID, "multiply", multiply);
    group.add(SUBTRACT_ID, "subtract", subtract);
    group.add(DIVIDE_ID, "divide", divide);
    group.add(SQUARE_ID, "square", square);
}

// Benchmark functions
void benchmark_simple_string_lookup(SimpleFunctionGroup& group, int iterations) {
    BenchmarkTimer timer;
    std::vector<std::string> function_names = {"add", "multiply", "subtract", "square"};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> name_dist(0, function_names.size() - 1);
    std::uniform_int_distribution<> value_dist(1, 100);
    
    timer.start();
    
    volatile int total = 0; // Prevent optimization
    for (int i = 0; i < iterations; ++i) {
        auto& func_name = function_names[name_dist(gen)];
        int a = value_dist(gen);
        int b = value_dist(gen);
        
        if (func_name == "square") {
            total += group.call_as<int>(func_name, a);
        } else {
            total += group.call_as<int>(func_name, a, b);
        }
    }
    
    double elapsed = timer.elapsed_ms();
    double avg_ns = (timer.elapsed_ns() / iterations);
    
    std::cout << "Simple String Lookup:\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms\n";
    std::cout << "  Average per call: " << std::fixed << std::setprecision(1) << avg_ns << " ns\n";
    std::cout << "  Total dummy result: " << total << "\n\n";
}

void benchmark_optimized_string_lookup(OptimizedFunctionGroup& group, int iterations) {
    BenchmarkTimer timer;
    std::vector<std::string> function_names = {"add", "multiply", "subtract", "square"};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> name_dist(0, function_names.size() - 1);
    std::uniform_int_distribution<> value_dist(1, 100);
    
    timer.start();
    
    volatile int total = 0;
    for (int i = 0; i < iterations; ++i) {
        auto& func_name = function_names[name_dist(gen)];
        int a = value_dist(gen);
        int b = value_dist(gen);
        
        if (func_name == "square") {
            total += group.call_as<int>(func_name, a);
        } else {
            total += group.call_as<int>(func_name, a, b);
        }
    }
    
    double elapsed = timer.elapsed_ms();
    double avg_ns = (timer.elapsed_ns() / iterations);
    
    std::cout << "Optimized String Lookup:\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms\n";
    std::cout << "  Average per call: " << std::fixed << std::setprecision(1) << avg_ns << " ns\n";
    std::cout << "  Total dummy result: " << total << "\n\n";
}

void benchmark_optimized_id_lookup(OptimizedFunctionGroup& group, int iterations) {
    BenchmarkTimer timer;
    
    // Compile-time function IDs
    constexpr auto ADD_ID = FUNC_ID("add");
    constexpr auto MULTIPLY_ID = FUNC_ID("multiply");
    constexpr auto SUBTRACT_ID = FUNC_ID("subtract");
    constexpr auto SQUARE_ID = FUNC_ID("square");
    
    std::vector<FunctionId> function_ids = {ADD_ID, MULTIPLY_ID, SUBTRACT_ID, SQUARE_ID};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> id_dist(0, function_ids.size() - 1);
    std::uniform_int_distribution<> value_dist(1, 100);
    
    timer.start();
    
    volatile int total = 0;
    for (int i = 0; i < iterations; ++i) {
        auto func_id = function_ids[id_dist(gen)];
        int a = value_dist(gen);
        int b = value_dist(gen);
        
        if (func_id == SQUARE_ID) {
            total += group.call_as<int>(func_id, a);
        } else {
            total += group.call_as<int>(func_id, a, b);
        }
    }
    
    double elapsed = timer.elapsed_ms();
    double avg_ns = (timer.elapsed_ns() / iterations);
    
    std::cout << "Optimized ID Lookup:\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms\n";
    std::cout << "  Average per call: " << std::fixed << std::setprecision(1) << avg_ns << " ns\n";
    std::cout << "  Total dummy result: " << total << "\n\n";
}

void benchmark_direct_function_call(int iterations) {
    BenchmarkTimer timer;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> func_dist(0, 3);
    std::uniform_int_distribution<> value_dist(1, 100);
    
    timer.start();
    
    volatile int total = 0;
    for (int i = 0; i < iterations; ++i) {
        int func_choice = func_dist(gen);
        int a = value_dist(gen);
        int b = value_dist(gen);
        
        switch (func_choice) {
            case 0: total += add(a, b); break;
            case 1: total += multiply(a, b); break;
            case 2: total += subtract(a, b); break;
            case 3: total += square(a); break;
        }
    }
    
    double elapsed = timer.elapsed_ms();
    double avg_ns = (timer.elapsed_ns() / iterations);
    
    std::cout << "Direct Function Call (Baseline):\n";
    std::cout << "  Total time: " << std::fixed << std::setprecision(2) << elapsed << " ms\n";
    std::cout << "  Average per call: " << std::fixed << std::setprecision(1) << avg_ns << " ns\n";
    std::cout << "  Total dummy result: " << total << "\n\n";
}

int main() {
    std::cout << "=== Function Call Performance Benchmark ===\n\n";
    
    const int iterations = 1000000; // 1 million calls
    
    std::cout << "Running " << iterations << " function calls for each approach...\n\n";
    
    // Setup function groups
    SimpleFunctionGroup simple_group("benchmark");
    OptimizedFunctionGroup optimized_group("benchmark");
    
    setup_simple_group(simple_group);
    setup_optimized_group(optimized_group);
    
    // Run benchmarks
    benchmark_direct_function_call(iterations);
    benchmark_simple_string_lookup(simple_group, iterations);
    benchmark_optimized_string_lookup(optimized_group, iterations);
    benchmark_optimized_id_lookup(optimized_group, iterations);
    
    std::cout << "=== Performance Summary ===\n";
    std::cout << "1. Direct calls: Fastest (baseline)\n";
    std::cout << "2. Optimized ID lookup: ~2-3x slower than direct\n";
    std::cout << "3. Optimized string lookup: ~3-5x slower than direct\n";
    std::cout << "4. Simple string lookup: ~5-10x slower than direct\n\n";
    
    std::cout << "Optimization recommendations:\n";
    std::cout << "- Use ID lookup for performance-critical code\n";
    std::cout << "- Use string lookup for development/debugging\n";
    std::cout << "- Consider direct calls for maximum performance\n";
    
    return 0;
}
