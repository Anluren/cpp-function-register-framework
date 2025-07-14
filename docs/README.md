# C++ Function Registration Framework

A flexible C++ framework that allows each .cpp file to register groups of functions to a `std::map`, where the key is an enum and the value is an object of a class containing functions.

## Features

- **Modular Function Registration**: Each .cpp file can register its own group of functions
- **Type-Safe Enum Keys**: Uses enums to identify different function groups
- **Flexible Function Signatures**: Supports functions with 0, 1, or 2 parameters
- **Automatic Registration**: Functions are registered automatically during static initialization
- **Cross-Platform**: Compatible with C++98 and later standards

## Quick Start - Simplified Modern API (C++17+)

For C++17+ projects, we recommend using the simplified modern API for the best developer experience:

```cpp
#include "simple_modern_function_group.h"
#include <iostream>

int main() {
    // Create function group
    auto group = std::make_unique<SimpleFunctionGroup>("Math");
    
    // Register any callable with unified API
    group->add("add", [](int a, int b) { return a + b; });
    group->add("square", [](int x) { return x * x; });
    
    // Type-safe function calls
    auto sum = group->call_as<int>("add", 10, 20);        // 30
    auto square = group->call_as<int>("square", 5);       // 25
    
    std::cout << "add(10, 20) = " << sum << std::endl;
    std::cout << "square(5) = " << square << std::endl;
    
    return 0;
}
```

**Benefits of the Modern API:**
- ✅ Unified `add()` method for function pointers, lambdas, and std::function
- ✅ Type-safe calls with automatic casting
- ✅ Modern C++17 features (`std::any`, `std::optional`, `if constexpr`)
- ✅ Minimal boilerplate code
- ✅ Built-in error handling with optional returns

See [SIMPLE_MODERN_API.md](SIMPLE_MODERN_API.md) for complete documentation.

## Modular Architecture

For larger projects, you can split function registration across multiple `.cpp` files:

```cpp
// math_function_group.cpp
void register_math_functions() {
    auto group = std::make_unique<SimpleFunctionGroup>("Math");
    group->add("add", [](int a, int b) { return a + b; });
    // ... register other math functions
    
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::MATH_FUNCTIONS, std::move(group));
}

// main.cpp
int main() {
    register_math_functions();    // Each .cpp registers its own group
    register_string_functions();
    register_utility_functions();
    
    // Use functions from any group
    auto& registry = SimpleFunctionRegistry::instance();
    auto* math = registry.get_group(FunctionGroupType::MATH_FUNCTIONS);
    auto result = math->call_as<int>("add", 10, 20);
    
    return 0;
}
```

See [MODULAR_REGISTRATION.md](MODULAR_REGISTRATION.md) for complete modular architecture guide.

## Legacy API (C++98+)

The original API supports older C++ standards:

## Architecture

### Core Components

1. **FunctionGroup**: Abstract base class for function groups
2. **ConcreteFunctionGroup**: Implementation that stores function pointers
3. **FunctionRegistry**: Singleton that manages all function groups
4. **FunctionGroupType**: Enum to identify different function groups

### Directory Structure

```
func_register/
├── function_group.h          # Function group interface and implementation
├── function_group.cpp        # Function group implementation
├── function_registry.h       # Registry interface
├── function_registry.cpp     # Registry implementation
├── math_functions.cpp        # Example math functions
├── string_functions.cpp      # Example string functions
├── main.cpp                  # Demo application
├── Makefile                  # Build configuration
└── README.md                 # This file
```

## Usage

### 1. Define Function Group Types

Add your custom function group types to the enum in `function_group.h`:

```cpp
enum FunctionGroupType {
    MATH_FUNCTIONS,
    STRING_FUNCTIONS,
    UTILITY_FUNCTIONS,
    YOUR_CUSTOM_GROUP,    // Add your groups here
    ANOTHER_GROUP
};
```

### 2. Create Functions

Create functions that match the supported signatures:

```cpp
// Function with no parameters
void* my_function_0() {
    static int result = 42;
    return &result;
}

// Function with one parameter
void* my_function_1(void* param) {
    int input = *static_cast<int*>(param);
    static int result = input * 2;
    return &result;
}

// Function with two parameters
void* my_function_2(void* param1, void* param2) {
    int a = *static_cast<int*>(param1);
    int b = *static_cast<int*>(param2);
    static int result = a + b;
    return &result;
}
```

### 3. Register Function Group

In your .cpp file, create and register a function group:

```cpp
#include "function_registry.h"

void initYourFunctions() {
    ConcreteFunctionGroup* group = new ConcreteFunctionGroup("Your Functions");
    
    // Register functions
    group->registerFunction0("function0", my_function_0);
    group->registerFunction1("function1", my_function_1);
    group->registerFunction2("function2", my_function_2);
    
    // Register the group
    FunctionRegistry::getInstance().registerGroup(YOUR_CUSTOM_GROUP, group);
}

// Automatic registration using static initialization
static class YourFunctionInitializer {
public:
    YourFunctionInitializer() {
        initYourFunctions();
    }
} yourInit;
```

### 4. Use Registered Functions

Access and call registered functions:

```cpp
#include "function_registry.h"

int main() {
    FunctionRegistry& registry = FunctionRegistry::getInstance();
    
    // Get a function group
    FunctionGroup* group = registry.getGroup(YOUR_CUSTOM_GROUP);
    
    if (group && group->hasFunction("function1")) {
        int input = 10;
        void* result = group->callFunction("function1", &input);
        int output = *static_cast<int*>(result);
        std::cout << "Result: " << output << std::endl;
    }
    
    return 0;
}
```

## Building and Running

### Prerequisites

- C++ compiler supporting C++98 or later (g++, clang++, etc.)
- Make utility

### Build Commands

```bash
# Build the project
make

# Build and run
make run

# Build with debug symbols
make debug

# Clean build files
make clean

# Show help
make help
```

### Manual Build

If you prefer not to use make:

```bash
g++ -Wall -Wextra -std=c++98 -O2 -o func_register_demo \
    main.cpp function_group.cpp function_registry.cpp \
    math_functions.cpp string_functions.cpp
```

## Example Output

```
=== Function Registration Framework Demo ===

Registered function groups: 2

=== Testing Math Functions ===
Group name: Math Functions
Available functions: add, multiply, power, sqrt
add() = 8
sqrt(16) = 4
multiply(6, 7) = 42
power(2, 3) = 8

=== Testing String Functions ===
Group name: String Functions
Available functions: concat, length, reverse, upper
length("Hello World") = 11
upper("hello world") = "HELLO WORLD"
reverse("hello") = "olleh"
concat("Hello ", "World!") = "Hello World!"

=== Framework Demo Complete ===
```

## Advanced Usage

### Custom Function Group Types

You can extend the enum and add your own function groups:

```cpp
enum FunctionGroupType {
    // ... existing types ...
    DATABASE_FUNCTIONS,
    NETWORK_FUNCTIONS,
    UI_FUNCTIONS
};
```

### Error Handling

The framework returns `nullptr` for invalid function calls:

```cpp
FunctionGroup* group = registry.getGroup(MATH_FUNCTIONS);
if (group) {
    if (group->hasFunction("sqrt")) {
        double input = 25.0;
        void* result = group->callFunction("sqrt", &input);
        if (result) {
            double output = *static_cast<double*>(result);
            std::cout << "sqrt(25) = " << output << std::endl;
        }
    }
}
```

### Memory Management

The framework manages memory automatically:
- Function groups are deleted when the registry is destroyed
- Static initialization ensures proper cleanup
- No manual memory management required for basic usage

## Limitations

1. **Return Type**: All functions must return `void*` for flexibility
2. **Parameter Passing**: Complex parameter passing requires careful void* handling
3. **Type Safety**: Limited compile-time type checking due to void* usage
4. **Function Signatures**: Currently supports 0, 1, or 2 parameters only

## Extending the Framework

To add support for more parameters or different function signatures:

1. Add new function pointer types in `function_group.h`
2. Add corresponding maps in `ConcreteFunctionGroup`
3. Update registration and calling methods
4. Modify the `callFunction` implementation

## Contributing

Feel free to extend this framework with additional features:
- More parameter support
- Better type safety
- Template-based function registration
- Exception handling
- Thread safety

## License

This framework is provided as-is for educational and development purposes.

## Performance Analysis

String-based function lookups in the framework have been thoroughly analyzed and benchmarked. Key findings:

### Benchmark Results
- **Direct function calls**: 48.8 ns (baseline)
- **ID-based lookup**: 224.2 ns (4.6x overhead)
- **String-based lookup**: 234-249 ns (4.8-5.1x overhead)

### Performance Recommendations
1. **Use string-based lookup by default** - Easy to debug and maintain
2. **Profile your application** - Identify actual performance bottlenecks
3. **Optimize hot paths with ID lookup** - Only where measurably beneficial
4. **Consider direct calls for critical paths** - When dynamic dispatch isn't needed

The performance difference between ID and string lookup is smaller than expected (~4.6x vs ~4.8x). The main bottleneck is function wrapping overhead, not the lookup mechanism itself.

For detailed analysis, see:
- [`docs/PERFORMANCE_ANALYSIS.md`](docs/PERFORMANCE_ANALYSIS.md) - Theoretical analysis
- [`docs/BENCHMARK_RESULTS.md`](docs/BENCHMARK_RESULTS.md) - Actual measurements
- [`examples/performance_benchmark.cpp`](examples/performance_benchmark.cpp) - Raw benchmarks
- [`examples/performance_comparison.cpp`](examples/performance_comparison.cpp) - Practical examples

### When to Optimize
- **High-frequency calls**: > 10,000 calls/second
- **Performance-critical code**: Real-time systems, game loops
- **Large function sets**: > 100 functions per group
- **Memory-constrained environments**: Embedded systems

For most applications, the string-based approach provides the best balance of usability and performance.
