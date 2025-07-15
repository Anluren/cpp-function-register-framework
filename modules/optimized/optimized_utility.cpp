#include "optimized_registry.h"
#include <chrono>
#include <random>

namespace Optimized {

// High-performance utility functions
const char* get_version_fast() {
    return "Optimized Function Registry v3.0 (C++17)";
}

// Fast timestamp using chrono
long long current_timestamp_fast() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now().time_since_epoch()
    ).count();
}

// Fast random number generation with thread-local generators
int random_fast() {
    static thread_local std::mt19937 gen(std::random_device{}());
    return static_cast<int>(gen());
}

int random_range_fast(int min, int max) {
    if (max <= min) return min;
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

// Fast hash function
int hash_fast(const char* str) {
    int hash = 0;
    while (*str) {
        hash = hash * 31 + *str++;
    }
    return hash;
}

// Fast memory usage placeholder
int memory_usage_fast() {
    return 42; // Placeholder for actual memory usage calculation
}

// Auto-registration class
class OptimizedUtilityRegistrar {
public:
    OptimizedUtilityRegistrar() {
        // Register the utility group
        OPTIMIZED_REGISTER_GROUP(FunctionCategory::UTILITY, "Optimized Utility Functions");
        
        // Register individual functions
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::UTILITY, "version", get_version_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::UTILITY, "timestamp", current_timestamp_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::UTILITY, "random", random_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::UTILITY, "random_range", random_range_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::UTILITY, "hash", hash_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::UTILITY, "memory_usage", memory_usage_fast);
    }
};

// Static instance to trigger registration
static OptimizedUtilityRegistrar utilityRegistrar;

} // namespace Optimized
