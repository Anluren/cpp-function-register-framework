#include "simple_modern_function_group.h"
#include <iostream>
#include <algorithm>

// String function implementations
std::string greet(const std::string& name) { 
    return "Hello, " + name + "!"; 
}

void print_message(const std::string& msg) { 
    std::cout << msg << std::endl; 
}

std::string uppercase(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
    }
    return result;
}

std::string repeat(const std::string& str, int count) {
    std::string result;
    for (int i = 0; i < count; ++i) {
        result += str;
    }
    return result;
}

// Function to register string functions
void register_string_functions() {
    auto string_group = std::make_unique<SimpleFunctionGroup>("String Operations");
    
    // Register function pointers
    string_group->add("greet", greet);
    string_group->add("print", print_message);
    string_group->add("uppercase", uppercase);
    string_group->add("repeat", repeat);
    
    // Register lambdas for additional string operations
    string_group->add("reverse", [](const std::string& str) {
        std::string result = str;
        std::reverse(result.begin(), result.end());
        return result;
    });
    
    string_group->add("concat", [](const std::string& a, const std::string& b) { 
        return a + " " + b; 
    });
    
    string_group->add("length", [](const std::string& str) {
        return static_cast<int>(str.length());
    });
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::STRING_FUNCTIONS, std::move(string_group));
}

// Automatic registration using static initialization
// This ensures the function group is registered when the module is loaded
namespace {
    struct StringFunctionAutoRegistration {
        StringFunctionAutoRegistration() {
            register_string_functions();
        }
    };
    
    // Static instance triggers registration at program startup
    static StringFunctionAutoRegistration auto_register;
}
