#include "modern_function_group.h"
#include <iostream>
#include <string>

// Example functions with different signatures
int add(int a, int b) { 
    return a + b; 
}

double multiply(double a, double b) { 
    return a * b; 
}

std::string greet(const std::string& name) { 
    return "Hello, " + name + "!"; 
}

void print_message(const std::string& msg) { 
    std::cout << msg << std::endl; 
}

int get_random() { 
    return 42; 
}

int main() {
    std::cout << "=== Modern C++17 Function Registration Demo ===" << std::endl;
    
    // Create and register function groups manually for clarity
    auto math_group = std::make_unique<ModernFunctionGroup>("Math Operations");
    math_group->add("add", add);
    math_group->add("multiply", multiply);
    math_group->add("random", get_random);
    
    auto string_group = std::make_unique<ModernFunctionGroup>("String Operations");
    string_group->add("greet", greet);
    string_group->add("print", print_message);
    
    // Register with the registry
    auto& registry = FunctionRegistry::instance();
    auto* math_ptr = math_group.get();
    auto* string_ptr = string_group.get();
    registry.register_group(FunctionGroupType::MATH_FUNCTIONS, std::move(math_group));
    registry.register_group(FunctionGroupType::STRING_FUNCTIONS, std::move(string_group));
    
    // Test math functions
    std::cout << "\n" << math_ptr->name() << ":" << std::endl;
    
    // Type-safe function calls
    auto sum = math_ptr->call_as<int>("add", 15, 25);
    std::cout << "add(15, 25) = " << sum << std::endl;
    
    auto product = math_ptr->call_as<double>("multiply", 3.14, 2.0);
    std::cout << "multiply(3.14, 2.0) = " << product << std::endl;
    
    auto random = math_ptr->call_as<int>("random");
    std::cout << "random() = " << random << std::endl;
    
    // Using try_call for safe calls
    if (auto result = math_ptr->try_call<int>("add", 10, 20)) {
        std::cout << "Safe add(10, 20) = " << *result << std::endl;
    }
    
    // Test string functions
    std::cout << "\n" << string_ptr->name() << ":" << std::endl;
    
    auto greeting = string_ptr->call_as<std::string>("greet", std::string("World"));
    std::cout << "greet(\"World\") = " << greeting << std::endl;
    
    // Void function call
    string_ptr->call_as<void>("print", std::string("This is a printed message"));
    
    // List all functions
    std::cout << "Available functions: ";
    for (const auto& name : string_ptr->function_names()) {
        std::cout << name << " ";
    }
    std::cout << std::endl;
    
    // Test with lambdas
    auto test_group = std::make_unique<ModernFunctionGroup>("Test Group");
    test_group->add("square", [](int x) { return x * x; });
    test_group->add("concat", [](const std::string& a, const std::string& b) { 
        return a + " " + b; 
    });
    
    auto square_result = test_group->call_as<int>("square", 5);
    std::cout << "\nLambda square(5) = " << square_result << std::endl;
    
    auto concat_result = test_group->call_as<std::string>("concat", 
                                                         std::string("Hello"), 
                                                         std::string("Lambda"));
    std::cout << "Lambda concat(\"Hello\", \"Lambda\") = " << concat_result << std::endl;
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}
