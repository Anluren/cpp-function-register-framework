#include "simple_modern_function_group.h"
#include "function_groups.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "=== Modular Function Registration Demo ===" << std::endl;
    std::cout << "Each function group is auto-registered from a separate .cpp file" << std::endl;
    std::cout << "(No manual registration calls needed - happens at program startup)" << std::endl;
    
    // Get registry and test functions from each group
    auto& registry = SimpleFunctionRegistry::instance();
    
    // Test Math Functions
    std::cout << "\n=== Math Functions ===" << std::endl;
    auto* math_group = registry.get_group(FunctionGroupType::MATH_FUNCTIONS);
    if (math_group) {
        std::cout << "Group: " << math_group->name() << std::endl;
        
        auto sum = math_group->call_as<int>("add", 15, 25);
        std::cout << "add(15, 25) = " << sum << std::endl;
        
        auto product = math_group->call_as<double>("multiply", 3.14, 2.0);
        std::cout << "multiply(3.14, 2.0) = " << product << std::endl;
        
        auto square = math_group->call_as<int>("square", 7);
        std::cout << "square(7) = " << square << std::endl;
        
        auto factorial = math_group->call_as<int>("factorial", 5);
        std::cout << "factorial(5) = " << factorial << std::endl;
        
        // List all math functions
        std::cout << "Available math functions: ";
        for (const auto& name : math_group->function_names()) {
            std::cout << name << " ";
        }
        std::cout << std::endl;
    }
    
    // Test String Functions
    std::cout << "\n=== String Functions ===" << std::endl;
    auto* string_group = registry.get_group(FunctionGroupType::STRING_FUNCTIONS);
    if (string_group) {
        std::cout << "Group: " << string_group->name() << std::endl;
        
        auto greeting = string_group->call_as<std::string>("greet", std::string("World"));
        std::cout << "greet(\"World\") = " << greeting << std::endl;
        
        auto upper = string_group->call_as<std::string>("uppercase", std::string("hello"));
        std::cout << "uppercase(\"hello\") = " << upper << std::endl;
        
        auto concat = string_group->call_as<std::string>("concat", 
                                                         std::string("Hello"), 
                                                         std::string("C++"));
        std::cout << "concat(\"Hello\", \"C++\") = " << concat << std::endl;
        
        auto length = string_group->call_as<int>("length", std::string("testing"));
        std::cout << "length(\"testing\") = " << length << std::endl;
        
        // Demonstrate void function
        std::cout << "Calling print function: ";
        string_group->call_as<void>("print", std::string("This message is from the string group!"));
        
        // List all string functions
        std::cout << "Available string functions: ";
        for (const auto& name : string_group->function_names()) {
            std::cout << name << " ";
        }
        std::cout << std::endl;
    }
    
    // Test Utility Functions
    std::cout << "\n=== Utility Functions ===" << std::endl;
    auto* utility_group = registry.get_group(FunctionGroupType::UTILITY_FUNCTIONS);
    if (utility_group) {
        std::cout << "Group: " << utility_group->name() << std::endl;
        
        auto fib = utility_group->call_as<int>("fibonacci", 10);
        std::cout << "fibonacci(10) = " << fib << std::endl;
        
        auto even = utility_group->call_as<bool>("is_even", 42);
        std::cout << "is_even(42) = " << (even ? "true" : "false") << std::endl;
        
        auto max_val = utility_group->call_as<int>("max", 15, 23);
        std::cout << "max(15, 23) = " << max_val << std::endl;
        
        auto abs_val = utility_group->call_as<int>("abs", -17);
        std::cout << "abs(-17) = " << abs_val << std::endl;
        
        auto random_b = utility_group->call_as<bool>("random_bool");
        std::cout << "random_bool() = " << (random_b ? "true" : "false") << std::endl;
        
        // Demonstrate void function
        std::cout << "Calling hello_world function: ";
        utility_group->call_as<void>("hello_world");
        
        // List all utility functions
        std::cout << "Available utility functions: ";
        for (const auto& name : utility_group->function_names()) {
            std::cout << name << " ";
        }
        std::cout << std::endl;
    }
    
    // Demonstrate safe calls with error handling
    std::cout << "\n=== Safe Function Calls ===" << std::endl;
    if (math_group) {
        // Successful call
        if (auto result = math_group->try_call<int>("add", 10, 20)) {
            std::cout << "Safe add(10, 20) = " << *result << std::endl;
        }
        
        // Failed call (function doesn't exist)
        if (auto result = math_group->try_call<int>("nonexistent", 5)) {
            std::cout << "This shouldn't print" << std::endl;
        } else {
            std::cout << "Safe call to nonexistent function failed gracefully" << std::endl;
        }
    }
    
    // Show total registered groups
    std::cout << "\n=== Registry Summary ===" << std::endl;
    auto group_types = registry.group_types();
    std::cout << "Total registered function groups: " << group_types.size() << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "This demonstrates modular function registration where:" << std::endl;
    std::cout << "- math_function_group.cpp registers math functions" << std::endl;
    std::cout << "- string_function_group.cpp registers string functions" << std::endl;
    std::cout << "- utility_function_group.cpp registers utility functions" << std::endl;
    std::cout << "- Each .cpp file is independent and self-contained" << std::endl;
    
    return 0;
}
