# Multi-Module Function Registry

## Overview

The Multi-Module Function Registry extends the basic function registration framework to support **multiple modules implementing the same function ID**, with the ability for callers to choose which specific module's implementation to use.

## Key Features

### 1. Multiple Implementations per Function
- Any number of modules can implement the same function ID
- Each implementation can have different algorithms, optimizations, or behaviors
- Registry tracks all implementations and which modules provide them

### 2. Caller-Controlled Module Selection
- Callers can specify exactly which module's implementation to use
- Provides `call_function<ReturnType>(module_name, function_id, args...)`
- Also supports `call_function_any<ReturnType>(function_id, args...)` for first available

### 3. Implementation Discovery
- Query which modules implement a specific function
- Query which functions a specific module implements
- Get overview of all implementations across the registry

### 4. Type-Safe Function Calls
- Template-based calls with compile-time type checking
- Automatic argument and return type conversion via `std::any`
- Robust error handling with `std::optional` return types

## Architecture

```cpp
// Standard function identifiers across all modules
enum class StandardFunctionId {
    ADD, SUBTRACT, MULTIPLY, DIVIDE,
    SQUARE, SQRT, POWER, ABS,
    TO_UPPER, TO_LOWER, CONCAT, LENGTH, REVERSE,
    IS_EVEN, FIBONACCI, RANDOM_INT, SLEEP_MS
};

// Function implementation info
struct FunctionImplementation {
    std::string module_name;
    std::string description;
    std::function<std::any(const std::vector<std::any>&)> function;
};

// Multi-module registry (singleton)
class MultiModuleFunctionRegistry {
    // Map: FunctionId -> Vector of implementations
    std::map<StandardFunctionId, std::vector<std::unique_ptr<FunctionImplementation>>> function_implementations_;
    
    // Map: ModuleName -> Functions it implements
    std::map<std::string, std::vector<StandardFunctionId>> module_functions_;
};
```

## Usage Examples

### Module Registration

```cpp
// BasicMath module - simple implementations
namespace BasicMath {
    int add(int a, int b) {
        return a + b;
    }
    
    void register_functions() {
        auto& registry = MultiModuleFunctionRegistry::instance();
        registry.register_function("BasicMath", StandardFunctionId::ADD, 
                                  "Simple addition", add);
    }
}

// OptimizedMath module - optimized implementations
namespace OptimizedMath {
    int add(int a, int b) {
        // Hypothetical SIMD-optimized addition
        return simd_add(a, b);
    }
    
    void register_functions() {
        auto& registry = MultiModuleFunctionRegistry::instance();
        registry.register_function("OptimizedMath", StandardFunctionId::ADD,
                                  "SIMD-optimized addition", add);
    }
}

// Automatic registration via static initialization
namespace {
    struct AutoRegistration {
        AutoRegistration() {
            BasicMath::register_functions();
            OptimizedMath::register_functions();
        }
    };
    static AutoRegistration auto_reg;
}
```

### Calling Functions from Specific Modules

```cpp
auto& registry = MultiModuleFunctionRegistry::instance();

// Call BasicMath implementation
auto result1 = registry.call_function<int>("BasicMath", StandardFunctionId::ADD, 5, 3);
if (result1) {
    std::cout << "BasicMath result: " << *result1 << std::endl;
}

// Call OptimizedMath implementation  
auto result2 = registry.call_function<int>("OptimizedMath", StandardFunctionId::ADD, 5, 3);
if (result2) {
    std::cout << "OptimizedMath result: " << *result2 << std::endl;
}

// Call first available implementation
auto result3 = registry.call_function_any<int>(StandardFunctionId::ADD, 5, 3);
if (result3) {
    std::cout << "Any implementation result: " << *result3 << std::endl;
}
```

### Discovery and Introspection

```cpp
// Find which modules implement a function
auto providers = registry.get_function_providers(StandardFunctionId::ADD);
for (const auto& module : providers) {
    std::cout << "ADD implemented by: " << module << std::endl;
}

// Find which functions a module implements
auto functions = registry.get_module_functions("BasicMath");
for (const auto& func_id : functions) {
    std::cout << "BasicMath implements: " << to_string(func_id) << std::endl;
}

// Check if specific module implements a function
bool has_sqrt = registry.module_has_function("BasicMath", StandardFunctionId::SQRT);

// Get all registered modules
auto all_modules = registry.get_all_modules();

// Show complete overview
registry.show_implementation_overview();
```

## Practical Use Cases

### 1. Performance Optimization
```cpp
// Debug mode: use detailed logging implementation
auto result = registry.call_function<int>("DebugMath", StandardFunctionId::MULTIPLY, a, b);

// Release mode: use optimized implementation  
auto result = registry.call_function<int>("OptimizedMath", StandardFunctionId::MULTIPLY, a, b);

// Fallback: use any available implementation
auto result = registry.call_function_any<int>(StandardFunctionId::MULTIPLY, a, b);
```

### 2. Algorithm Selection
```cpp
// Choose algorithm based on data size
if (data_size > LARGE_THRESHOLD) {
    // Use parallel implementation
    result = registry.call_function<double>("ParallelMath", StandardFunctionId::SORT, data);
} else {
    // Use simple implementation
    result = registry.call_function<double>("BasicMath", StandardFunctionId::SORT, data);
}
```

### 3. Vendor-Specific Implementations
```cpp
// Use hardware-accelerated implementation if available
if (registry.module_has_function("CudaMath", StandardFunctionId::MATRIX_MULTIPLY)) {
    result = registry.call_function<Matrix>("CudaMath", StandardFunctionId::MATRIX_MULTIPLY, a, b);
} else if (registry.module_has_function("IntelMKL", StandardFunctionId::MATRIX_MULTIPLY)) {
    result = registry.call_function<Matrix>("IntelMKL", StandardFunctionId::MATRIX_MULTIPLY, a, b);
} else {
    result = registry.call_function_any<Matrix>(StandardFunctionId::MATRIX_MULTIPLY, a, b);
}
```

### 4. Feature-Specific Implementations
```cpp
// Choose implementation based on required features
if (need_high_precision) {
    result = registry.call_function<BigDecimal>("HighPrecisionMath", StandardFunctionId::DIVIDE, a, b);
} else if (need_fast_approximation) {
    result = registry.call_function<float>("FastMath", StandardFunctionId::DIVIDE, a, b);
} else {
    result = registry.call_function<double>("StandardMath", StandardFunctionId::DIVIDE, a, b);
}
```

## Error Handling

The registry provides robust error handling:

```cpp
auto result = registry.call_function<int>("NonExistentModule", StandardFunctionId::ADD, 1, 2);
if (!result) {
    std::cout << "Module not found or function not implemented" << std::endl;
}

// Check availability before calling
if (registry.module_has_function("MyModule", StandardFunctionId::SQRT)) {
    auto sqrt_result = registry.call_function<double>("MyModule", StandardFunctionId::SQRT, 25);
} else {
    std::cout << "MyModule doesn't implement SQRT" << std::endl;
}
```

## Performance Characteristics

### Function Lookup
- **Module-specific calls**: O(log n) for function lookup + O(m) for module search within function
  - n = number of unique functions
  - m = average number of modules per function
- **Any-module calls**: O(log n) for function lookup + O(1) for first implementation

### Memory Usage
- **Per function**: Storage for multiple implementation pointers
- **Per module**: List of function IDs it implements
- **Overhead**: Minimal due to efficient std::map and std::vector storage

### Registration
- **Time**: O(log n) for function insertion + O(1) for module tracking
- **Space**: Linear in number of implementations

## Comparison with Single-Implementation Registry

| Feature | Single Implementation | Multi-Module Implementation |
|---------|---------------------|---------------------------|
| Function Lookup | O(log n) | O(log n + m) |
| Memory per Function | 1 implementation | m implementations |
| Module Selection | Not supported | Full control |
| Fallback Options | Not supported | Multiple options |
| Implementation Discovery | Basic | Comprehensive |

## Demo Program

Run the multi-module demo to see all features in action:

```bash
cd build
./examples/multi_module_demo
```

The demo shows:
1. Multiple modules registering the same functions
2. Different algorithms and optimizations for each implementation
3. Caller-controlled module selection
4. Function and module discovery
5. Error handling for missing modules/functions
6. Registry overview and analysis

## Integration

To use the multi-module registry in your project:

1. Include the header:
   ```cpp
   #include "multi_module_registry.h"
   ```

2. Register your modules:
   ```cpp
   auto& registry = MultiModuleFunctionRegistry::instance();
   registry.register_function("YourModule", StandardFunctionId::YOUR_FUNCTION, 
                             "Description", your_function);
   ```

3. Call functions with module selection:
   ```cpp
   auto result = registry.call_function<ReturnType>("YourModule", 
                                                   StandardFunctionId::YOUR_FUNCTION, 
                                                   args...);
   ```

The multi-module registry is fully compatible with the existing single-implementation registries and can be used alongside them in the same project.
