# Constrained Function Registration: Limiting Modules to Predefined Function Sets

## Overview

This document explains different approaches to constrain function registration so that:
1. **You define a fixed set of possible functions** (the interface)
2. **Each module implements only a subset** of those functions
3. **The system ensures type safety** and interface compliance
4. **Runtime checking** determines which functions are available

## Approach 1: Simple Set-Based Validation

The easiest approach using your existing framework:

### Define the Interface

```cpp
// math_interface.h
#include <set>
#include <string>

namespace math_interface {
    // Define all allowed math functions
    const std::set<std::string> ALLOWED_FUNCTIONS = {
        "add", "subtract", "multiply", "divide",
        "square", "sqrt", "power", "abs", "sin", "cos"
    };
    
    // Helper function to validate
    bool is_valid_function(const std::string& name) {
        return ALLOWED_FUNCTIONS.find(name) != ALLOWED_FUNCTIONS.end();
    }
}
```

### Create Constrained Module

```cpp
// constrained_math_module.cpp
#include "math_interface.h"
#include "simple_modern_function_group.h"

class ConstrainedMathModule {
private:
    std::unique_ptr<SimpleFunctionGroup> group_;
    std::vector<std::string> implemented_;
    
public:
    ConstrainedMathModule(const std::string& module_name) 
        : group_(std::make_unique<SimpleFunctionGroup>(module_name)) {}
    
    // Only allow registration of predefined functions
    template<typename Func>
    bool register_function(const std::string& name, Func&& func) {
        if (!math_interface::is_valid_function(name)) {
            std::cerr << "ERROR: '" << name << "' not in math interface!" << std::endl;
            return false;
        }
        
        group_->add(name, std::forward<Func>(func));
        implemented_.push_back(name);
        std::cout << "Registered: " << name << std::endl;
        return true;
    }
    
    // Show implementation status
    void show_status() const {
        std::cout << "Implementation Status:" << std::endl;
        for (const auto& func : math_interface::ALLOWED_FUNCTIONS) {
            bool has = group_->has_function(func);
            std::cout << "  " << func << ": " << (has ? "✓" : "✗") << std::endl;
        }
    }
    
    SimpleFunctionGroup* get_group() { return group_.get(); }
};

// Usage in module
void register_basic_math_module() {
    ConstrainedMathModule module("BasicMath");
    
    // These will succeed (in the interface)
    module.register_function("add", [](int a, int b) { return a + b; });
    module.register_function("multiply", [](int a, int b) { return a * b; });
    
    // This will fail (not in the interface)
    module.register_function("invalid_func", [](int x) { return x; });
    
    module.show_status();
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::MATH_FUNCTIONS, 
                           std::unique_ptr<SimpleFunctionGroup>(module.get_group()));
}
```

## Approach 2: Enum-Based Function IDs

Use enums to ensure compile-time safety:

### Define Function Interface

```cpp
// function_interface.h
enum class MathFunction {
    ADD = 0,
    SUBTRACT = 1,
    MULTIPLY = 2,
    DIVIDE = 3,
    SQUARE = 4,
    SQRT = 5,
    POWER = 6,
    ABS = 7
};

enum class StringFunction {
    TO_UPPER = 0,
    TO_LOWER = 1,
    CONCAT = 2,
    LENGTH = 3,
    REVERSE = 4
};

// Helper to convert enum to string
std::string to_string(MathFunction func) {
    switch (func) {
        case MathFunction::ADD: return "add";
        case MathFunction::SUBTRACT: return "subtract";
        case MathFunction::MULTIPLY: return "multiply";
        // ... etc
        default: return "unknown";
    }
}
```

### Constrained Module Implementation

```cpp
template<typename FunctionEnum>
class TypedFunctionModule {
private:
    SimpleFunctionGroup group_;
    std::set<FunctionEnum> implemented_;
    
public:
    TypedFunctionModule(const std::string& name) : group_(name) {}
    
    // Type-safe registration using enum
    template<typename Func>
    void register_function(FunctionEnum func_id, Func&& func) {
        std::string name = to_string(func_id);
        group_.add(name, std::forward<Func>(func));
        implemented_.insert(func_id);
        std::cout << "Registered: " << name << std::endl;
    }
    
    // Check if function is implemented
    bool has_function(FunctionEnum func_id) const {
        return implemented_.find(func_id) != implemented_.end();
    }
    
    // Type-safe function calling
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> call_function(FunctionEnum func_id, Args&&... args) {
        if (!has_function(func_id)) {
            return std::nullopt;
        }
        
        std::string name = to_string(func_id);
        try {
            return group_.call_as<ReturnType>(name, std::forward<Args>(args)...);
        } catch (...) {
            return std::nullopt;
        }
    }
};

// Usage
void register_typed_math_module() {
    TypedFunctionModule<MathFunction> math_module("TypedMath");
    
    // Type-safe registration - compiler enforces enum values
    math_module.register_function(MathFunction::ADD, [](int a, int b) { return a + b; });
    math_module.register_function(MathFunction::MULTIPLY, [](int a, int b) { return a * b; });
    
    // Usage with type safety
    auto result = math_module.call_function<int>(MathFunction::ADD, 10, 20);
    if (result) {
        std::cout << "ADD(10, 20) = " << *result << std::endl;
    }
}
```

## Approach 3: Interface Registry Pattern

Multiple specialized registries for different function types:

### Separate Registries

```cpp
// Specialized registries for different function types
class MathFunctionRegistry : public SimpleFunctionRegistry {
private:
    static const std::set<std::string> ALLOWED_MATH_FUNCTIONS;
    
public:
    bool register_math_function(const std::string& module_name, 
                               const std::string& func_name, 
                               auto func) {
        if (ALLOWED_MATH_FUNCTIONS.find(func_name) == ALLOWED_MATH_FUNCTIONS.end()) {
            return false; // Function not in math interface
        }
        
        // Create or get module group
        auto* group = get_or_create_group(module_name);
        group->add(func_name, func);
        return true;
    }
    
    // Math-specific convenience methods
    std::optional<int> add(int a, int b) {
        return call_any_provider<int>("add", a, b);
    }
    
    std::optional<double> multiply(double a, double b) {
        return call_any_provider<double>("multiply", a, b);
    }
    
private:
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> call_any_provider(const std::string& func_name, Args&&... args) {
        // Try each registered module until one has the function
        for (auto& [module_name, group] : groups_) {
            if (group->has_function(func_name)) {
                try {
                    return group->call_as<ReturnType>(func_name, std::forward<Args>(args)...);
                } catch (...) {
                    continue; // Try next provider
                }
            }
        }
        return std::nullopt;
    }
};

// Similarly for string functions
class StringFunctionRegistry : public SimpleFunctionRegistry {
    // ... string-specific interface and methods
};
```

## Approach 4: Configuration-Driven Interface

Define interfaces in configuration files:

### Interface Definition

```json
// math_interface.json
{
  "interface_name": "MathFunctions",
  "version": "1.0",
  "functions": {
    "add": {
      "parameters": ["int", "int"],
      "return_type": "int",
      "description": "Add two integers"
    },
    "multiply": {
      "parameters": ["double", "double"], 
      "return_type": "double",
      "description": "Multiply two doubles"
    },
    "square": {
      "parameters": ["int"],
      "return_type": "int", 
      "description": "Square an integer"
    }
  }
}
```

### Runtime Interface Validation

```cpp
class ConfigurableInterface {
private:
    std::map<std::string, FunctionSignature> allowed_functions_;
    
public:
    bool load_interface(const std::string& config_file) {
        // Parse JSON and populate allowed_functions_
        // Return true if successful
    }
    
    bool is_valid_function(const std::string& name, 
                          const std::vector<std::string>& param_types,
                          const std::string& return_type) {
        auto it = allowed_functions_.find(name);
        if (it == allowed_functions_.end()) return false;
        
        // Validate signature matches
        return it->second.matches(param_types, return_type);
    }
    
    template<typename Func>
    bool register_function(const std::string& name, Func&& func) {
        // Runtime type checking against interface
        if (!is_valid_function(name, get_param_types<Func>(), get_return_type<Func>())) {
            return false;
        }
        
        // Register if valid
        function_group_.add(name, std::forward<Func>(func));
        return true;
    }
};
```

## Practical Example: Plugin System

Here's how you might use this in a real plugin system:

### Define Core Interface

```cpp
// plugin_interface.h
namespace plugin_interface {
    enum class CoreFunction {
        PROCESS_DATA,
        VALIDATE_INPUT,
        FORMAT_OUTPUT,
        GET_VERSION,
        GET_DESCRIPTION
    };
    
    // Each plugin must implement these core functions
    const std::set<CoreFunction> REQUIRED_FUNCTIONS = {
        CoreFunction::PROCESS_DATA,
        CoreFunction::GET_VERSION
    };
    
    // Optional functions
    const std::set<CoreFunction> OPTIONAL_FUNCTIONS = {
        CoreFunction::VALIDATE_INPUT,
        CoreFunction::FORMAT_OUTPUT, 
        CoreFunction::GET_DESCRIPTION
    };
}
```

### Plugin Implementation

```cpp
// image_processor_plugin.cpp
class ImageProcessorPlugin {
public:
    void register_functions() {
        ConstrainedPluginModule plugin("ImageProcessor");
        
        // Required functions
        plugin.register_function(CoreFunction::PROCESS_DATA, 
            [](const std::string& data) { return process_image(data); });
        plugin.register_function(CoreFunction::GET_VERSION, 
            []() { return std::string("1.0.0"); });
            
        // Optional functions
        plugin.register_function(CoreFunction::VALIDATE_INPUT,
            [](const std::string& input) { return is_valid_image(input); });
            
        // Validate that all required functions are implemented
        if (!plugin.validate_required_functions()) {
            throw std::runtime_error("Plugin missing required functions");
        }
        
        PluginRegistry::instance().register_plugin("ImageProcessor", std::move(plugin));
    }
};
```

## Benefits of Each Approach

| Approach | Pros | Cons | Best For |
|----------|------|------|----------|
| **Set-based** | Simple, flexible | Runtime checking only | Small interfaces |
| **Enum-based** | Compile-time safety | Requires enum updates | Stable interfaces |
| **Registry pattern** | Type-specific organization | More complex | Large systems |
| **Config-driven** | Maximum flexibility | Runtime overhead | Dynamic systems |

## Recommendations

1. **Start with Set-based validation** for simplicity
2. **Use Enum-based** for performance-critical or stable interfaces  
3. **Use Registry pattern** for large systems with multiple function types
4. **Use Config-driven** for maximum flexibility and runtime configuration

The approach you choose depends on your specific requirements for type safety, performance, and flexibility.
