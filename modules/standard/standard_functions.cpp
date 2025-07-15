#include "standard_registry.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <sstream>
#include <functional>

namespace Standard {

// Standard function implementations
namespace StandardFunctions {

// Math functions
std::function<int(int, int)> add = [](int a, int b) { return a + b; };
std::function<int(int, int)> multiply = [](int a, int b) { return a * b; };
std::function<double(double)> square_root = [](double x) { return std::sqrt(x); };
std::function<double(double, double)> power = [](double base, double exp) { return std::pow(base, exp); };

// String functions
std::function<std::string(const std::string&)> to_uppercase = [](const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
};

std::function<size_t(const std::string&)> string_length = [](const std::string& str) {
    return str.length();
};

std::function<std::string(const std::string&, const std::string&)> string_concat = [](const std::string& a, const std::string& b) {
    return a + b;
};

// Utility functions
std::function<std::string()> get_version = []() {
    return "Standard Registry v1.0";
};

std::function<int(const std::vector<int>&)> vector_sum = [](const std::vector<int>& vec) {
    return std::accumulate(vec.begin(), vec.end(), 0);
};

std::function<void(const std::string&)> print_message = [](const std::string& msg) {
    std::cout << "[Standard] " << msg << std::endl;
};

} // namespace StandardFunctions

// Initialization function
void initStandardFunctions() {
    auto& registry = Standard::StandardRegistry::getInstance();
    
    // Register math functions
    registry.registerFunction("add", StandardFunctions::add);
    registry.registerFunction("multiply", StandardFunctions::multiply);
    registry.registerFunction("sqrt", StandardFunctions::square_root);
    registry.registerFunction("power", StandardFunctions::power);
    
    // Register string functions
    registry.registerFunction("to_upper", StandardFunctions::to_uppercase);
    registry.registerFunction("length", StandardFunctions::string_length);
    registry.registerFunction("concat", StandardFunctions::string_concat);
    
    // Register utility functions
    registry.registerFunction("version", StandardFunctions::get_version);
    registry.registerFunction("vector_sum", StandardFunctions::vector_sum);
    registry.registerFunction("print", StandardFunctions::print_message);
}

} // namespace Standard
