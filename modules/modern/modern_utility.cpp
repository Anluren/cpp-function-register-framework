#include "modern_registry.h"
#include <chrono>
#include <random>
#include <thread>
#include <future>
#include <sstream>

namespace Modern {

// Modern utility functions using C++11 threading and chrono
namespace UtilityFunctions {

// Time functions using chrono
auto current_timestamp = []() -> long long {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
};

auto format_duration = [](long long milliseconds) -> std::string {
    auto duration = std::chrono::milliseconds(milliseconds);
    auto hours = std::chrono::duration_cast<std::chrono::hours>(duration);
    duration -= hours;
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(duration);
    duration -= minutes;
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(duration);
    
    std::ostringstream oss;
    oss << hours.count() << "h " << minutes.count() << "m " << seconds.count() << "s";
    return oss.str();
};

// Random number generation with modern random facilities
auto random_int = [](int min, int max) -> int {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
};

auto random_double = []() -> double {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(gen);
};

auto random_double_range = [](double min, double max) -> double {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(min, max);
    return dis(gen);
};

auto random_bool = []() -> bool {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    std::bernoulli_distribution dis(0.5);
    return dis(gen);
};

auto random_bool_prob = [](double probability) -> bool {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    std::bernoulli_distribution dis(probability);
    return dis(gen);
};

// Utility functions
auto get_version = []() -> std::string {
    return "Modern Function Registry v2.0 (C++11)";
};

auto cpu_count = []() -> unsigned int {
    return std::thread::hardware_concurrency();
};

// Asynchronous function execution
auto async_factorial = [](int n) -> std::future<long long> {
    return std::async(std::launch::async, [n]() -> long long {
        long long result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate work
        return result;
    });
};

auto sleep_ms = [](int milliseconds) -> void {
    std::this_thread::sleep_for(std::chrono::milliseconds(milliseconds));
};

// Template-based generic utilities
template<typename T>
auto clamp_value = [](T value, T min_val, T max_val) -> T {
    return std::max(min_val, std::min(value, max_val));
};

// Hash function
auto hash_string = [](const std::string& str) -> size_t {
    return std::hash<std::string>{}(str);
};

// Memory usage
auto memory_usage = []() -> std::string {
    return "Memory usage information not available on this platform";
};

} // namespace UtilityFunctions

// Registration class
class ModernUtilityRegistrar {
public:
    ModernUtilityRegistrar() {
        // Register the utility group
        MODERN_REGISTER_GROUP(FunctionCategory::UTILITY, "Modern Utility Functions");
        
        auto* registry = &ModernRegistry::getInstance();
        
        // Register time functions
        registry->registerLambda<long long()>(FunctionCategory::UTILITY, "current_timestamp", UtilityFunctions::current_timestamp);
        registry->registerLambda<std::string(long long)>(FunctionCategory::UTILITY, "format_duration", UtilityFunctions::format_duration);
        
        // Register random functions
        registry->registerLambda<int(int, int)>(FunctionCategory::UTILITY, "random_int", UtilityFunctions::random_int);
        registry->registerLambda<double()>(FunctionCategory::UTILITY, "random_double", UtilityFunctions::random_double);
        registry->registerLambda<double(double, double)>(FunctionCategory::UTILITY, "random_double_range", UtilityFunctions::random_double_range);
        registry->registerLambda<bool()>(FunctionCategory::UTILITY, "random_bool", UtilityFunctions::random_bool);
        registry->registerLambda<bool(double)>(FunctionCategory::UTILITY, "random_bool_prob", UtilityFunctions::random_bool_prob);
        
        // Register system info
        registry->registerLambda<std::string()>(FunctionCategory::UTILITY, "version", UtilityFunctions::get_version);
        registry->registerLambda<unsigned int()>(FunctionCategory::UTILITY, "cpu_count", UtilityFunctions::cpu_count);
        registry->registerLambda<std::string()>(FunctionCategory::UTILITY, "memory_usage", UtilityFunctions::memory_usage);
        
        // Register async functions
        registry->registerLambda<std::future<long long>(int)>(FunctionCategory::UTILITY, "async_factorial", UtilityFunctions::async_factorial);
        registry->registerLambda<void(int)>(FunctionCategory::UTILITY, "sleep", UtilityFunctions::sleep_ms);
        
        // Register hash function
        registry->registerLambda<size_t(const std::string&)>(FunctionCategory::UTILITY, "hash_string", UtilityFunctions::hash_string);
        
        // Register template functions (with specific types)
        auto clamp_int = [](int value, int min_val, int max_val) -> int {
            return std::max(min_val, std::min(value, max_val));
        };
        auto clamp_double = [](double value, double min_val, double max_val) -> double {
            return std::max(min_val, std::min(value, max_val));
        };
        
        registry->registerLambda<int(int, int, int)>(FunctionCategory::UTILITY, "clamp_int", clamp_int);
        registry->registerLambda<double(double, double, double)>(FunctionCategory::UTILITY, "clamp_double", clamp_double);
    }
};

// Static instance for auto-registration
static ModernUtilityRegistrar utilityRegistrar;

// Explicit initialization function for static linking
void initUtilityFunctions() {
    static bool initialized = false;
    if (!initialized) {
        // Force the static registrar to be instantiated
        static ModernUtilityRegistrar registrar;
        initialized = true;
    }
}

} // namespace Modern
