#include "simple_modern_function_group.h"
#include <iostream>
#include <set>
#include <vector>

/**
 * Practical Example: Constrained Function Registration
 * 
 * This demonstrates how to limit modules to implement only functions
 * from a predefined interface, using your existing framework.
 */

// Define the standard math interface (what functions are allowed)
namespace math_interface {
    const std::set<std::string> STANDARD_FUNCTIONS = {
        "add", "subtract", "multiply", "divide",
        "square", "sqrt", "power", "abs"
    };
    
    bool is_standard_function(const std::string& name) {
        return STANDARD_FUNCTIONS.find(name) != STANDARD_FUNCTIONS.end();
    }
    
    void show_interface() {
        std::cout << "Standard Math Interface Functions:" << std::endl;
        for (const auto& func : STANDARD_FUNCTIONS) {
            std::cout << "  - " << func << std::endl;
        }
    }
}

// Enhanced function group that enforces interface compliance
class InterfaceCompliantMathGroup : public SimpleFunctionGroup {
private:
    std::vector<std::string> implemented_functions_;
    std::string module_name_;
    
public:
    explicit InterfaceCompliantMathGroup(const std::string& module_name)
        : SimpleFunctionGroup("Math: " + module_name), module_name_(module_name) {}
    
    // Constrained function registration
    template<typename Func>
    bool register_function(const std::string& name, Func&& func) {
        if (!math_interface::is_standard_function(name)) {
            std::cout << "[" << module_name_ << "] ERROR: Function '" << name 
                      << "' is not part of the standard math interface!" << std::endl;
            return false;
        }
        
        if (has_function(name)) {
            std::cout << "[" << module_name_ << "] WARNING: Function '" << name 
                      << "' already implemented, overriding..." << std::endl;
        }
        
        add(name, std::forward<Func>(func));
        implemented_functions_.push_back(name);
        std::cout << "[" << module_name_ << "] ✓ Registered: " << name << std::endl;
        return true;
    }
    
    // Show implementation status vs interface
    void show_implementation_status() const {
        std::cout << std::endl << "[" << module_name_ << "] Implementation Status:" << std::endl;
        std::cout << "Module implements " << implemented_functions_.size() 
                  << "/" << math_interface::STANDARD_FUNCTIONS.size() 
                  << " standard functions" << std::endl;
                  
        for (const auto& std_func : math_interface::STANDARD_FUNCTIONS) {
            bool implemented = has_function(std_func);
            std::cout << "  " << std_func << ": " 
                      << (implemented ? "✓ IMPLEMENTED" : "✗ NOT IMPLEMENTED") << std::endl;
        }
    }
    
    const std::vector<std::string>& get_implemented_functions() const {
        return implemented_functions_;
    }
    
    const std::string& get_module_name() const {
        return module_name_;
    }
};

// Example Module 1: Basic Math Operations
namespace basic_math_module {
    int add_impl(int a, int b) { return a + b; }
    int subtract_impl(int a, int b) { return a - b; }
    int multiply_impl(int a, int b) { return a * b; }
    int abs_impl(int x) { return x < 0 ? -x : x; }
    
    void register_basic_math() {
        auto module = std::make_unique<InterfaceCompliantMathGroup>("BasicMath");
        
        std::cout << "=== Registering Basic Math Module ===" << std::endl;
        
        // Register valid functions (subset of interface)
        module->register_function("add", add_impl);
        module->register_function("subtract", subtract_impl);
        module->register_function("multiply", multiply_impl);
        module->register_function("abs", abs_impl);
        
        // Try to register invalid function (should fail)
        module->register_function("invalid_operation", [](int x) { return x; });
        
        module->show_implementation_status();
        
        auto& registry = SimpleFunctionRegistry::instance();
        registry.register_group(FunctionGroupType::CUSTOM_GROUP_1, std::move(module));
    }
}

// Example Module 2: Advanced Math Operations  
namespace advanced_math_module {
    double divide_impl(double a, double b) { return b != 0.0 ? a / b : 0.0; }
    int square_impl(int x) { return x * x; }
    double sqrt_impl(double x) { 
        // Simple sqrt implementation
        if (x < 0) return 0;
        double guess = x / 2.0;
        for (int i = 0; i < 10; ++i) {
            guess = (guess + x / guess) / 2.0;
        }
        return guess;
    }
    
    void register_advanced_math() {
        auto module = std::make_unique<InterfaceCompliantMathGroup>("AdvancedMath");
        
        std::cout << std::endl << "=== Registering Advanced Math Module ===" << std::endl;
        
        // Register different subset of interface
        module->register_function("divide", divide_impl);
        module->register_function("square", square_impl);
        module->register_function("sqrt", sqrt_impl);
        
        // Try to register function from different domain (should fail)
        module->register_function("string_length", [](const std::string& s) { 
            return static_cast<int>(s.length()); 
        });
        
        module->show_implementation_status();
        
        auto& registry = SimpleFunctionRegistry::instance();
        registry.register_group(FunctionGroupType::CUSTOM_GROUP_2, std::move(module));
    }
}

// Function to test the constrained system
void test_constrained_system() {
    std::cout << std::endl << "=== Testing Constrained Function System ===" << std::endl;
    
    auto& registry = SimpleFunctionRegistry::instance();
    
    // Test basic math functions
    auto* basic_math = registry.get_group(FunctionGroupType::CUSTOM_GROUP_1);
    if (basic_math) {
        std::cout << std::endl << "Testing Basic Math Module:" << std::endl;
        
        auto sum = basic_math->call_as<int>("add", 15, 25);
        std::cout << "add(15, 25) = " << sum << std::endl;
        
        auto diff = basic_math->call_as<int>("subtract", 25, 15);
        std::cout << "subtract(25, 15) = " << diff << std::endl;
        
        // Try function not implemented by this module
        try {
            auto sqrt_result = basic_math->call_as<double>("sqrt", 16.0);
            std::cout << "sqrt(16) = " << sqrt_result << std::endl;
        } catch (const std::exception& e) {
            std::cout << "sqrt not available in BasicMath: " << e.what() << std::endl;
        }
    }
    
    // Test advanced math functions
    auto* advanced_math = registry.get_group(FunctionGroupType::CUSTOM_GROUP_2);
    if (advanced_math) {
        std::cout << std::endl << "Testing Advanced Math Module:" << std::endl;
        
        auto quotient = advanced_math->call_as<double>("divide", 20.0, 4.0);
        std::cout << "divide(20, 4) = " << quotient << std::endl;
        
        auto square = advanced_math->call_as<int>("square", 7);
        std::cout << "square(7) = " << square << std::endl;
        
        auto sqrt_result = advanced_math->call_as<double>("sqrt", 16.0);
        std::cout << "sqrt(16) = " << sqrt_result << std::endl;
    }
}

int main() {
    std::cout << "=== Constrained Function Registration Demo ===" << std::endl;
    std::cout << "Demonstrating how modules can only implement predefined functions" << std::endl;
    
    // Show the standard interface
    math_interface::show_interface();
    
    // Register modules (each implements subset of interface)
    basic_math_module::register_basic_math();
    advanced_math_module::register_advanced_math();
    
    // Test the system
    test_constrained_system();
    
    std::cout << std::endl << "=== Summary ===" << std::endl;
    std::cout << "Benefits of this approach:" << std::endl;
    std::cout << "1. ✓ Enforces interface compliance" << std::endl;
    std::cout << "2. ✓ Each module implements only what it supports" << std::endl;
    std::cout << "3. ✓ Clear separation of concerns" << std::endl;
    std::cout << "4. ✓ Runtime validation of function availability" << std::endl;
    std::cout << "5. ✓ Easy to extend interface by adding to the standard set" << std::endl;
    
    return 0;
}
