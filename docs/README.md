# C++ Function Registration Framework

A flexible C++ framework that allows each .cpp file to register groups of functions to a `std::map`, where the key is an enum and the value is an object of a class containing functions.

## Features

- **Modular Function Registration**: Each .cpp file can register its own group of functions
- **Type-Safe Enum Keys**: Uses enums to identify different function groups
- **Flexible Function Signatures**: Supports functions with 0, 1, or 2 parameters
- **Automatic Registration**: Functions are registered automatically during static initialization
- **Cross-Platform**: Compatible with C++98 and later standards

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
