# Modular Function Registration Guide

This guide demonstrates how to split your function registration into multiple `.cpp` files, where each file registers its own function group independently.

## Architecture

The modular approach separates concerns by having:
- **One header file** (`function_groups.h`) that declares registration functions
- **Multiple .cpp files** - each implementing one registration function for a specific domain
- **One main file** that calls all registration functions and demonstrates usage

## File Structure

```
examples/
├── function_groups.h           # Registration function declarations
├── math_function_group.cpp     # Math functions implementation & registration
├── string_function_group.cpp   # String functions implementation & registration  
├── utility_function_group.cpp  # Utility functions implementation & registration
└── modular_demo.cpp           # Main demo that uses all groups
```

## Implementation Pattern

### 1. Header File (`function_groups.h`)

```cpp
#pragma once

// Function declarations for registering different function groups
void register_math_functions();
void register_string_functions();
void register_utility_functions();
```

### 2. Function Group Implementation (e.g., `math_function_group.cpp`)

```cpp
#include "simple_modern_function_group.h"

// 1. Implement your functions
int add(int a, int b) { 
    return a + b; 
}

double multiply(double a, double b) { 
    return a * b; 
}

// 2. Registration function
void register_math_functions() {
    auto math_group = std::make_unique<SimpleFunctionGroup>("Math Operations");
    
    // Register function pointers
    math_group->add("add", add);
    math_group->add("multiply", multiply);
    
    // Register lambdas
    math_group->add("square", [](int x) { return x * x; });
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::MATH_FUNCTIONS, std::move(math_group));
}
```

### 3. Main Application (`modular_demo.cpp`)

```cpp
#include "simple_modern_function_group.h"
#include "function_groups.h"

int main() {
    // Register all function groups
    register_math_functions();
    register_string_functions();
    register_utility_functions();
    
    // Use the functions
    auto& registry = SimpleFunctionRegistry::instance();
    auto* math_group = registry.get_group(FunctionGroupType::MATH_FUNCTIONS);
    
    auto result = math_group->call_as<int>("add", 10, 20);
    std::cout << "Result: " << result << std::endl;
    
    return 0;
}
```

## Benefits of Modular Approach

### ✅ **Separation of Concerns**
- Each .cpp file focuses on one specific domain (math, strings, utilities)
- Functions are grouped logically
- Easy to understand and maintain

### ✅ **Independent Development**
- Teams can work on different function groups simultaneously
- Changes to math functions don't affect string functions
- Each file can be tested independently

### ✅ **Compile-Time Efficiency**
- Only changed function groups need recompilation
- Faster incremental builds
- Better for large projects

### ✅ **Modular Deployment**
- Can conditionally include/exclude function groups
- Easy to create different builds with different feature sets
- Plugin-like architecture

### ✅ **Code Organization**
- Clear file naming convention
- Related functions stay together
- Easy to find and modify specific functionality

## Example Function Groups

### Math Functions (`math_function_group.cpp`)
```cpp
void register_math_functions() {
    auto group = std::make_unique<SimpleFunctionGroup>("Math Operations");
    
    // Basic arithmetic
    group->add("add", [](int a, int b) { return a + b; });
    group->add("multiply", [](double a, double b) { return a * b; });
    
    // Advanced math
    group->add("square", [](int x) { return x * x; });
    group->add("factorial", [](int n) { /* implementation */ return result; });
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::MATH_FUNCTIONS, std::move(group));
}
```

### String Functions (`string_function_group.cpp`)
```cpp
void register_string_functions() {
    auto group = std::make_unique<SimpleFunctionGroup>("String Operations");
    
    // String manipulation
    group->add("greet", [](const std::string& name) { 
        return "Hello, " + name + "!"; 
    });
    group->add("uppercase", uppercase_function);
    group->add("concat", [](const std::string& a, const std::string& b) { 
        return a + " " + b; 
    });
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::STRING_FUNCTIONS, std::move(group));
}
```

### Utility Functions (`utility_function_group.cpp`)
```cpp
void register_utility_functions() {
    auto group = std::make_unique<SimpleFunctionGroup>("Utility Functions");
    
    // General utilities
    group->add("fibonacci", fibonacci_function);
    group->add("is_even", [](int n) { return n % 2 == 0; });
    group->add("max", [](int a, int b) { return (a > b) ? a : b; });
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::UTILITY_FUNCTIONS, std::move(group));
}
```

## CMake Integration

Add to your `CMakeLists.txt`:

```cmake
# Modular demo executable
add_executable(modular_demo 
    modular_demo.cpp
    math_function_group.cpp
    string_function_group.cpp
    utility_function_group.cpp
)
target_link_libraries(modular_demo function_register_core)
target_include_directories(modular_demo 
    PRIVATE ${CMAKE_SOURCE_DIR}/include
)
```

## Best Practices

### 📁 **File Naming Convention**
- `[domain]_function_group.cpp` for implementation files
- `function_groups.h` for registration function declarations
- `[app_name]_demo.cpp` or `main.cpp` for the main application

### 🔧 **Function Naming**
- `register_[domain]_functions()` for registration functions
- Clear, descriptive names for actual functions
- Group related functions logically

### 🏗️ **Error Handling**
- Use `try_call()` for safe function invocation
- Check if function groups exist before using them
- Provide meaningful error messages

### 📚 **Documentation**
- Document each function group's purpose
- List available functions in comments
- Provide usage examples

## Running the Demo

```bash
# Build the project
./build.sh

# Run the modular demo
./build/examples/modular_demo
```

This will demonstrate:
- Registration of 3 separate function groups
- Function calls from each group
- Safe error handling
- Complete registry functionality

The modular approach scales well for large applications with many different types of functions, making the codebase more maintainable and allowing for better team collaboration.
