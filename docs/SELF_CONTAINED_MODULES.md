# Self-Contained Function Group Pattern

## Overview

This document explains how to create completely self-contained function group modules where the registration code is embedded directly in the same file as the function implementations.

## Pattern Structure

Each function group file follows this structure:

```cpp
// 1. Include necessary headers
#include "simple_modern_function_group.h"
#include <other_headers...>

// 2. Function implementations
int my_function(int a, int b) {
    return a + b;
}

// 3. Registration function
void register_my_functions() {
    auto group = std::make_unique<SimpleFunctionGroup>("My Functions");
    group->add("my_func", my_function);
    
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::MY_GROUP, std::move(group));
}

// 4. Automatic registration
namespace {
    struct MyFunctionAutoRegistration {
        MyFunctionAutoRegistration() {
            register_my_functions();
        }
    };
    static MyFunctionAutoRegistration auto_register;
}
```

## Benefits of This Pattern

### ✅ **Self-Contained Modules**
- Each `.cpp` file contains everything needed for one function group
- Functions and their registration are co-located
- Easy to understand and maintain
- Clear separation of concerns

### ✅ **Automatic Registration**
- No manual registration calls needed in `main()`
- Functions are available as soon as the module is linked
- Zero configuration required from the user
- Registration happens at program startup

### ✅ **Modular Architecture**
- Easy to add new function groups
- Easy to remove function groups (just don't link the file)
- Independent development of different function groups
- Clear module boundaries

### ✅ **Build System Friendly**
- CMake automatically includes linked `.cpp` files
- No need to maintain separate registration lists
- Clean dependencies

## Example: Complete Function Group

Here's a complete example of a self-contained database function group:

```cpp
// database_functions.cpp
#include "simple_modern_function_group.h"
#include <iostream>

namespace database {

// Function implementations
bool connect(const std::string& host) {
    std::cout << "Connecting to: " << host << std::endl;
    return true;
}

int count_records(const std::string& table) {
    std::cout << "Counting records in: " << table << std::endl;
    return 42;
}

std::string get_status() {
    return "Connected";
}

// Registration function
void register_database_functions() {
    auto db_group = std::make_unique<SimpleFunctionGroup>("Database");
    
    db_group->add("connect", connect);
    db_group->add("count", count_records);
    db_group->add("status", get_status);
    
    // Add lambda functions too
    db_group->add("ping", []() { return true; });
    
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::CUSTOM_GROUP_1, std::move(db_group));
}

} // namespace database

// Automatic registration
namespace {
    struct DatabaseAutoRegistration {
        DatabaseAutoRegistration() {
            database::register_database_functions();
        }
    };
    static DatabaseAutoRegistration auto_register;
}
```

## Usage

### Adding a New Function Group

1. **Create the `.cpp` file** with your functions and registration
2. **Add to CMakeLists.txt** (if using CMake)
3. **That's it!** - Functions are automatically available

### Using Registered Functions

```cpp
// No manual registration needed!
int main() {
    auto& registry = SimpleFunctionRegistry::instance();
    auto* db_group = registry.get_group(FunctionGroupType::CUSTOM_GROUP_1);
    
    auto connected = db_group->call_as<bool>("connect", "localhost");
    auto count = db_group->call_as<int>("count", "users");
    
    return 0;
}
```

### CMake Integration

```cmake
# Just add your function group file to the executable
add_executable(my_app
    main.cpp
    database_functions.cpp  # Automatically registers itself
    math_functions.cpp      # Automatically registers itself
    # ... other function groups
)
```

## Advanced Patterns

### Conditional Registration

```cpp
// Register only in debug builds
void register_debug_functions() {
#ifdef DEBUG
    auto debug_group = std::make_unique<SimpleFunctionGroup>("Debug");
    debug_group->add("debug_info", get_debug_info);
    
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::CUSTOM_GROUP_2, std::move(debug_group));
#endif
}
```

### Registration with Configuration

```cpp
void register_network_functions() {
    auto net_group = std::make_unique<SimpleFunctionGroup>("Network");
    
    // Configure based on environment
    if (std::getenv("ENABLE_SSL")) {
        net_group->add("connect_ssl", ssl_connect);
    }
    
    net_group->add("connect", basic_connect);
    
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::CUSTOM_GROUP_1, std::move(net_group));
}
```

### Namespace Organization

```cpp
namespace company::database {
    void register_functions() { /* ... */ }
}

namespace company::networking {
    void register_functions() { /* ... */ }
}

// Each can have its own auto-registration
```

## Best Practices

### ✅ **Do:**
- Keep related functions together in one file
- Use namespaces to avoid naming conflicts
- Include comprehensive error handling in functions
- Document function signatures and return types
- Use descriptive function and group names

### ❌ **Don't:**
- Mix unrelated functionality in one file
- Forget the auto-registration section
- Use the same FunctionGroupType for multiple files
- Make functions too dependent on global state

## Comparison with Other Patterns

| Pattern | Pros | Cons | Use Case |
|---------|------|------|----------|
| **Self-Contained** | Simple, modular, automatic | Slightly more boilerplate | **Recommended for most cases** |
| **Central Registry** | Single point of control | Must maintain registration list | Large teams, strict control |
| **Manual Registration** | Full control, explicit | Must remember to call | Simple applications |

## Migration Guide

### From Manual Registration:

**Before:**
```cpp
// main.cpp
int main() {
    register_math_functions();    // Manual calls
    register_string_functions();
    // ... rest of code
}
```

**After:**
```cpp
// main.cpp
int main() {
    // Functions automatically registered!
    // ... just use them
}
```

### From Central Registry:

**Before:**
```cpp
// central_registry.cpp
void register_all_functions() {
    register_math_functions();
    register_string_functions();
    // ... must maintain this list
}
```

**After:**
```cpp
// No central file needed!
// Each module registers itself
```

## Conclusion

The self-contained function group pattern provides the best balance of:
- **Simplicity**: Easy to understand and implement
- **Modularity**: Clear separation of concerns
- **Maintainability**: Co-located code and registration
- **Usability**: Zero configuration required

This pattern is recommended for most applications and scales well from small projects to large codebases.
