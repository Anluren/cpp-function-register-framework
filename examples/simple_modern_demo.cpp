#include "simple_modern_function_group.h"
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
    std::cout << "=== Simple Modern C++17 Function Registration Demo ===" << std::endl;
    
    // Create function groups
    auto math_group = std::make_unique<SimpleFunctionGroup>("Math Operations");
    auto string_group = std::make_unique<SimpleFunctionGroup>("String Operations");
    auto test_group = std::make_unique<SimpleFunctionGroup>("Test Group");
    
    // Register function pointers
    math_group->add("add", add);
    math_group->add("multiply", multiply);
    math_group->add("random", get_random);
    
    string_group->add("greet", greet);
    string_group->add("print", print_message);
    
    // Register lambdas (same API!)
    test_group->add("square", [](int x) { return x * x; });
    test_group->add("concat", [](const std::string& a, const std::string& b) { 
        return a + " " + b; 
    });
    test_group->add("hello_world", []() { 
        return std::string("Hello, World!"); 
    });
    
    // Register with the registry
    auto& registry = SimpleFunctionRegistry::instance();
    auto* math_ptr = math_group.get();
    auto* string_ptr = string_group.get();
    auto* test_ptr = test_group.get();
    
    registry.register_group(FunctionGroupType::MATH_FUNCTIONS, std::move(math_group));
    registry.register_group(FunctionGroupType::STRING_FUNCTIONS, std::move(string_group));
    registry.register_group(FunctionGroupType::UTILITY_FUNCTIONS, std::move(test_group));
    
    // Test function calls
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
    
    // Test lambda functions
    std::cout << "\n" << test_ptr->name() << ":" << std::endl;
    
    auto square_result = test_ptr->call_as<int>("square", 5);
    std::cout << "Lambda square(5) = " << square_result << std::endl;
    
    auto concat_result = test_ptr->call_as<std::string>("concat", 
                                                        std::string("Hello"), 
                                                        std::string("Lambda"));
    std::cout << "Lambda concat(\"Hello\", \"Lambda\") = " << concat_result << std::endl;
    
    auto hello_result = test_ptr->call_as<std::string>("hello_world");
    std::cout << "Lambda hello_world() = " << hello_result << std::endl;
    
    // List all functions
    std::cout << "\nAvailable functions:" << std::endl;
    for (const auto& name : test_ptr->function_names()) {
        std::cout << "  " << name << std::endl;
    }
    
    std::cout << "\n=== Demo Complete ===" << std::endl;
    return 0;
}
