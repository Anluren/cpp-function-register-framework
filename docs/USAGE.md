# How to Add New Function Groups

This guide shows you how to extend the function registration framework with your own function groups.

## Step 1: Add Your Enum Value

Edit `function_group.h` and add your function group type to the enum:

```cpp
enum FunctionGroupType {
    MATH_FUNCTIONS,
    STRING_FUNCTIONS,
    UTILITY_FUNCTIONS,
    CUSTOM_GROUP_1,
    CUSTOM_GROUP_2,
    YOUR_NEW_GROUP,    // Add this line
};
```

## Step 2: Create Your Functions File

Create a new `.cpp` file (e.g., `my_functions.cpp`):

```cpp
#include "function_registry.h"
#include <iostream>

// Example: A function that prints a message and returns success
void* print_hello() {
    std::cout << "Hello from your function!" << std::endl;
    static bool success = true;
    return &success;
}

// Example: A function that doubles a number
void* double_number(void* input) {
    int value = *static_cast<int*>(input);
    static int result = value * 2;
    return &result;
}

// Example: A function that adds two numbers
void* add_numbers(void* a_ptr, void* b_ptr) {
    int a = *static_cast<int*>(a_ptr);
    int b = *static_cast<int*>(b_ptr);
    static int result = a + b;
    return &result;
}

// Initialize your function group
void initMyFunctions() {
    ConcreteFunctionGroup* myGroup = new ConcreteFunctionGroup("My Custom Functions");
    
    // Register your functions
    myGroup->registerFunction0("hello", print_hello);
    myGroup->registerFunction1("double", double_number);
    myGroup->registerFunction2("add", add_numbers);
    
    // Register with the framework
    FunctionRegistry::getInstance().registerGroup(YOUR_NEW_GROUP, myGroup);
}

// Auto-register when program starts
static class MyFunctionInitializer {
public:
    MyFunctionInitializer() {
        initMyFunctions();
    }
} myInit;
```

## Step 3: Update the Build

Add your new file to the Makefile:

```makefile
SOURCES = main.cpp function_group.cpp function_registry.cpp \
          math_functions.cpp string_functions.cpp utility_functions.cpp \
          my_functions.cpp
```

## Step 4: Use Your Functions

In your main code or another file:

```cpp
#include "function_registry.h"

int main() {
    FunctionRegistry& registry = FunctionRegistry::getInstance();
    FunctionGroup* myGroup = registry.getGroup(YOUR_NEW_GROUP);
    
    if (myGroup) {
        // Call function with no parameters
        myGroup->callFunction("hello");
        
        // Call function with one parameter
        int input = 21;
        void* result = myGroup->callFunction("double", &input);
        int doubled = *static_cast<int*>(result);
        std::cout << "Double of 21 is: " << doubled << std::endl;
        
        // Call function with two parameters
        int a = 10, b = 20;
        void* args[2] = {&a, &b};
        void* sum_result = myGroup->callFunction("add", args);
        int sum = *static_cast<int*>(sum_result);
        std::cout << "10 + 20 = " << sum << std::endl;
    }
    
    return 0;
}
```

## Step 5: Build and Run

```bash
make clean
make
./func_register_demo
```

## Best Practices

### Function Return Values
- Always use `static` variables for return values to ensure they persist
- Return pointers to your static variables
- For complex return types, consider using heap allocation with proper cleanup

### Parameter Handling
- Always cast `void*` parameters to the correct type
- For multiple parameters, pass an array of `void*` pointers
- Validate parameters when necessary

### Memory Management
- The framework handles cleanup of function groups automatically
- Use static variables for simple return values
- For dynamic memory, ensure proper cleanup in your functions

### Error Handling
- Return `nullptr` for error conditions
- Check for null pointers when calling functions
- Use `hasFunction()` to verify function existence

## Advanced Examples

### String Handling
```cpp
void* process_string(void* str_ptr) {
    const char* input = static_cast<const char*>(str_ptr);
    static std::string result = "Processed: ";
    result = "Processed: " + std::string(input);
    return const_cast<char*>(result.c_str());
}
```

### Complex Data Structures
```cpp
struct Point {
    int x, y;
};

void* create_point(void* x_ptr, void* y_ptr) {
    int x = *static_cast<int*>(x_ptr);
    int y = *static_cast<int*>(y_ptr);
    static Point result;
    result.x = x;
    result.y = y;
    return &result;
}
```

### Array Processing
```cpp
void* sum_array(void* array_info) {
    // Expect first element to be array size, followed by array data
    int* data = static_cast<int*>(array_info);
    int size = data[0];
    static int result = 0;
    result = 0;
    for (int i = 1; i <= size; ++i) {
        result += data[i];
    }
    return &result;
}
```

This framework provides a flexible foundation for organizing and calling functions across multiple source files in a type-safe, efficient manner.
