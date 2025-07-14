# Simple Modern C++17 Function Registration API

This is a simplified version of the function registration framework that takes advantage of C++17 features for a cleaner, more unified API.

## Key Features

- **Unified registration**: Single `add()` method for function pointers, lambdas, and std::function objects
- **Type safety**: Template-based calls with automatic type deduction
- **Modern C++17**: Uses `std::any`, `std::optional`, structured bindings, and `if constexpr`
- **Simple error handling**: Optional-based safe calls and exception-based error reporting
- **Minimal boilerplate**: No need to manually specify function signatures

## Basic Usage

```cpp
#include "simple_modern_function_group.h"
#include <iostream>

// Example functions
int add(int a, int b) { return a + b; }
std::string greet(const std::string& name) { return "Hello, " + name; }

int main() {
    // Create a function group
    auto group = std::make_unique<SimpleFunctionGroup>("Math");
    
    // Register any callable with the same simple API
    group->add("add", add);                                    // Function pointer
    group->add("square", [](int x) { return x * x; });        // Lambda
    group->add("hello", []() { return "Hello World!"; });     // No-arg lambda
    
    // Type-safe function calls
    auto sum = group->call_as<int>("add", 10, 20);
    auto square = group->call_as<int>("square", 5);
    auto greeting = group->call_as<std::string>("hello");
    
    std::cout << "add(10, 20) = " << sum << std::endl;        // 30
    std::cout << "square(5) = " << square << std::endl;       // 25
    std::cout << "hello() = " << greeting << std::endl;       // Hello World!
    
    // Safe calls with optional return
    if (auto result = group->try_call<int>("add", 5, 15)) {
        std::cout << "Safe call result: " << *result << std::endl;
    }
    
    return 0;
}
```

## Registry Usage

```cpp
#include "simple_modern_function_group.h"

int main() {
    // Create groups
    auto math_group = std::make_unique<SimpleFunctionGroup>("Math");
    auto string_group = std::make_unique<SimpleFunctionGroup>("String");
    
    // Register functions
    math_group->add("add", [](int a, int b) { return a + b; });
    string_group->add("greet", [](const std::string& name) { 
        return "Hello, " + name; 
    });
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::MATH_FUNCTIONS, std::move(math_group));
    registry.register_group(FunctionGroupType::STRING_FUNCTIONS, std::move(string_group));
    
    // Use from registry
    auto* math = registry.get_group(FunctionGroupType::MATH_FUNCTIONS);
    auto result = math->call_as<int>("add", 10, 20);
    
    return 0;
}
```

## Supported Function Types

- **No arguments**: `T()`
- **One argument**: `T(Arg)` 
- **Two arguments**: `T(Arg1, Arg2)`
- **Function pointers**: `int (*func)(int, int)`
- **Lambdas**: `[](int a, int b) { return a + b; }`
- **std::function**: `std::function<int(int, int)>`

### Supported Parameter Types

- `int`
- `double` 
- `std::string`
- `const std::string&`

## API Reference

### SimpleFunctionGroup

```cpp
class SimpleFunctionGroup {
public:
    explicit SimpleFunctionGroup(std::string name);
    
    // Registration
    template<typename Func>
    void add(const std::string& name, Func&& func);
    
    // Information
    const std::string& name() const;
    std::vector<std::string> function_names() const;
    bool has_function(const std::string& name) const;
    
    // Function calls
    template<typename... Args>
    std::any call(const std::string& name, Args&&... args) const;
    
    template<typename ReturnType, typename... Args>
    ReturnType call_as(const std::string& name, Args&&... args) const;
    
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> try_call(const std::string& name, Args&&... args) const;
};
```

### SimpleFunctionRegistry

```cpp
class SimpleFunctionRegistry {
public:
    static SimpleFunctionRegistry& instance();
    
    void register_group(FunctionGroupType type, std::unique_ptr<SimpleFunctionGroup> group);
    SimpleFunctionGroup* get_group(FunctionGroupType type);
    std::vector<FunctionGroupType> group_types() const;
};
```

## Comparison with Original API

### Before (Original API)
```cpp
// Different methods for different function types
auto group = std::make_unique<ConcreteFunctionGroup>("Math");
group->registerFunction("add", add);  // Function pointer only
group->registerLambda("square", [](int x) { return x * x; });  // Lambda only

// Complex type erasure and manual casting
auto result = group->callFunction("add", {&a, &b});
int sum = *static_cast<int*>(result.getData());
```

### After (Simplified API)
```cpp
// Unified registration for any callable
auto group = std::make_unique<SimpleFunctionGroup>("Math");
group->add("add", add);                                    // Function pointer
group->add("square", [](int x) { return x * x; });        // Lambda - same method!

// Type-safe calls with automatic casting
auto sum = group->call_as<int>("add", 10, 20);
```

## Benefits

1. **Simpler**: One `add()` method instead of multiple registration methods
2. **Type-safe**: Template-based calls with compile-time type checking
3. **Modern**: Uses C++17 features like `std::any`, `if constexpr`, and structured bindings
4. **Unified**: Same API for function pointers, lambdas, and std::function objects
5. **Safe**: Optional-based error handling with `try_call()`
6. **Clean**: Minimal boilerplate and easy to read

This simplified API maintains all the power of the original framework while being much easier to use and understand.
