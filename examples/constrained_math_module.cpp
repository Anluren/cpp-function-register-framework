#include "simple_modern_function_group.h"
#include <iostream>
#include <set>

/**
 * Constrained Math Module - only implements a subset of predefined math functions
 * 
 * This demonstrates how to limit modules to implement only specific functions
 * from a predefined interface.
 */

namespace constrained_math_module {

// Define the allowed math functions (predefined interface)
const std::set<std::string> ALLOWED_MATH_FUNCTIONS = {
    "add",
    "subtract", 
    "multiply",
    "divide",
    "square",
    "sqrt",
    "power",
    "abs"
};

// Math function implementations (this module only implements some of them)
int add_impl(int a, int b) {
    return a + b;
}

int subtract_impl(int a, int b) {
    return a - b;
}

double multiply_impl(double a, double b) {
    return a * b;
}

int square_impl(int x) {
    return x * x;
}

// Helper class to enforce function constraints
class ConstrainedMathGroup : public SimpleFunctionGroup {
public:
    explicit ConstrainedMathGroup(const std::string& module_name) 
        : SimpleFunctionGroup(module_name) {}
    
    // Override add method to check against allowed functions
    template<typename Func>
    bool register_function(const std::string& name, Func&& func) {
        if (ALLOWED_MATH_FUNCTIONS.find(name) == ALLOWED_MATH_FUNCTIONS.end()) {
            std::cout << "ERROR: Function '" << name << "' is not in the predefined math interface!" << std::endl;
            return false;
        }
        
        add(name, std::forward<Func>(func));
        std::cout << "Successfully registered math function: " << name << std::endl;
        return true;
    }
    
    // Show which functions from the interface are implemented
    void show_implementation_status() const {
        std::cout << "Math Interface Implementation Status:" << std::endl;
        for (const auto& allowed_func : ALLOWED_MATH_FUNCTIONS) {
            bool implemented = has_function(allowed_func);
            std::cout << "  " << allowed_func << ": " 
                      << (implemented ? "IMPLEMENTED" : "NOT IMPLEMENTED") << std::endl;
        }
    }
};

// Module registration function
void register_constrained_math_module() {
    auto math_module = std::make_unique<ConstrainedMathGroup>("ConstrainedMathModule");
    
    std::cout << "Registering Constrained Math Module..." << std::endl;
    std::cout << "This module only implements a subset of the predefined math interface" << std::endl;
    
    // Register only the functions this module supports (subset of the interface)
    math_module->register_function("add", add_impl);
    math_module->register_function("subtract", subtract_impl);
    math_module->register_function("multiply", multiply_impl);
    math_module->register_function("square", square_impl);
    
    // Note: This module does NOT implement: divide, sqrt, power, abs
    // Other modules could implement those
    
    // Try to register a function not in the interface (this should fail)
    std::cout << std::endl << "Attempting to register invalid function..." << std::endl;
    math_module->register_function("invalid_function", [](int x) { return x; });
    
    std::cout << std::endl;
    math_module->show_implementation_status();
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::CUSTOM_GROUP_1, std::move(math_module));
    
    std::cout << std::endl << "Constrained Math Module registered with 4/8 functions implemented." << std::endl;
}

} // namespace constrained_math_module

// Automatic registration
namespace {
    struct ConstrainedMathModuleAutoRegistration {
        ConstrainedMathModuleAutoRegistration() {
            constrained_math_module::register_constrained_math_module();
        }
    };
    static ConstrainedMathModuleAutoRegistration auto_register;
}
