#include "modern_registry.h"
#include <cmath>
#include <algorithm>
#include <numeric>

namespace Modern {

// Modern math functions using lambdas and functional programming
namespace MathFunctions {

// Basic arithmetic
auto add = [](int a, int b) -> int { return a + b; };
auto subtract = [](int a, int b) -> int { return a - b; };
auto multiply = [](int a, int b) -> int { return a * b; };
auto divide = [](int a, int b) -> double { 
    return (b != 0) ? static_cast<double>(a) / b : 0.0; 
};

// Advanced math
auto power = [](double base, double exp) -> double { return std::pow(base, exp); };
auto sqrt_func = [](double x) -> double { return std::sqrt(x); };
auto sin_func = [](double x) -> double { return std::sin(x); };
auto cos_func = [](double x) -> double { return std::cos(x); };

// Vector operations using templates and lambdas
auto vector_sum = [](const std::vector<int>& vec) -> int {
    return std::accumulate(vec.begin(), vec.end(), 0);
};

auto vector_average = [](const std::vector<double>& vec) -> double {
    if (vec.empty()) return 0.0;
    return std::accumulate(vec.begin(), vec.end(), 0.0) / vec.size();
};

auto vector_max = [](const std::vector<int>& vec) -> int {
    return vec.empty() ? 0 : *std::max_element(vec.begin(), vec.end());
};

// Factorial using recursive lambda
std::function<int(int)> factorial = [](int n) -> int {
    return (n <= 1) ? 1 : n * factorial(n - 1);
};

// Fibonacci using memoization
auto fibonacci = [](int n) -> long long {
    static std::vector<long long> memo = {0, 1};
    if (n >= memo.size()) {
        for (size_t i = memo.size(); i <= n; ++i) {
            memo.push_back(memo[i-1] + memo[i-2]);
        }
    }
    return memo[n];
};

} // namespace MathFunctions

// Registration class
class ModernMathRegistrar {
public:
    ModernMathRegistrar() {
        // Register the math group
        MODERN_REGISTER_GROUP(FunctionCategory::MATH, "Modern Math Functions");
        
        auto* registry = &ModernRegistry::getInstance();
        
        // Register basic arithmetic
        registry->registerLambda<int(int, int)>(FunctionCategory::MATH, "add", MathFunctions::add);
        registry->registerLambda<int(int, int)>(FunctionCategory::MATH, "subtract", MathFunctions::subtract);
        registry->registerLambda<int(int, int)>(FunctionCategory::MATH, "multiply", MathFunctions::multiply);
        registry->registerLambda<double(int, int)>(FunctionCategory::MATH, "divide", MathFunctions::divide);
        
        // Register advanced math
        registry->registerLambda<double(double, double)>(FunctionCategory::MATH, "power", MathFunctions::power);
        registry->registerLambda<double(double)>(FunctionCategory::MATH, "sqrt", MathFunctions::sqrt_func);
        registry->registerLambda<double(double)>(FunctionCategory::MATH, "sin", MathFunctions::sin_func);
        registry->registerLambda<double(double)>(FunctionCategory::MATH, "cos", MathFunctions::cos_func);
        
        // Register vector operations
        registry->registerLambda<int(const std::vector<int>&)>(FunctionCategory::MATH, "vector_sum", MathFunctions::vector_sum);
        registry->registerLambda<double(const std::vector<double>&)>(FunctionCategory::MATH, "vector_average", MathFunctions::vector_average);
        registry->registerLambda<int(const std::vector<int>&)>(FunctionCategory::MATH, "vector_max", MathFunctions::vector_max);
        
        // Register special functions
        registry->registerFunction<int(int)>(FunctionCategory::MATH, "factorial", MathFunctions::factorial);
        registry->registerLambda<long long(int)>(FunctionCategory::MATH, "fibonacci", MathFunctions::fibonacci);
    }
};

// Static instance for auto-registration
static ModernMathRegistrar mathRegistrar;

// Explicit initialization function for static linking
void initMathFunctions() {
    static bool initialized = false;
    if (!initialized) {
        // Force the static registrar to be instantiated
        static ModernMathRegistrar registrar;
        initialized = true;
    }
}

} // namespace Modern
