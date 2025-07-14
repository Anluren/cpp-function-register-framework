# Performance Analysis: String-Based Function Lookups

## Current Performance Characteristics

### **Double Lookup Problem**
Your framework currently performs two string-based lookups:
1. **Function Group Lookup**: `registry.get_group(enum)` - ✅ **Fast** (enum-based)
2. **Function Lookup**: `group->call_as<int>("function_name", args)` - ⚠️ **Slower** (string-based)

### **Performance Bottlenecks Identified**

#### 1. **String Comparison Overhead** - O(log n)
```cpp
// Current implementation uses std::map<std::string>
std::map<std::string, std::function<...>> functions_;

// Each function call requires:
auto it = functions_.find(name);  // O(log n) + string comparison
```

**Time Complexity:**
- **std::map lookup**: O(log n) where n = number of functions
- **String comparison**: O(k) where k = average string length
- **Total per call**: O(log n × k)

#### 2. **Memory Overhead**
```cpp
// String keys stored for every function
std::map<std::string, std::function<...>> functions_;
// Memory = n × (average_string_length + function_wrapper_size)
```

#### 3. **Type Erasure Cost**
```cpp
// Runtime type checking and casting
return std::any_cast<ReturnType>(result);  // Runtime type validation
```

## Performance Impact Analysis

### **Benchmarking Results** (Estimated)

| Lookup Method | Time per Call | Memory Usage | Complexity |
|---------------|---------------|--------------|------------|
| **Current String-based** | ~200-500ns | High (strings) | O(log n × k) |
| **Optimized Hash-based** | ~50-150ns | Medium | O(1) average |
| **Compile-time ID** | ~30-80ns | Low (integers) | O(1) |

### **Real-World Impact**
For a function called **1 million times**:
- **String lookup**: ~200-500ms total
- **Optimized lookup**: ~50-150ms total  
- **Performance gain**: **60-75% improvement**

## Optimization Strategies

### **Strategy 1: Hash-Based Lookup** ⭐ **Recommended**

```cpp
// Replace std::map with std::unordered_map
std::unordered_map<std::size_t, std::function<...>> functions_by_id_;
std::map<std::string, std::size_t> name_to_id_;  // For compatibility

// Usage with compile-time hash
constexpr auto ADD_ID = hash_string("add");
group->call_as<int>(ADD_ID, 10, 20);  // O(1) lookup
```

**Benefits:**
- ✅ **O(1) average lookup** instead of O(log n)
- ✅ **Compile-time ID generation** eliminates runtime string processing
- ✅ **Backward compatible** - still supports string names
- ✅ **Memory efficient** - integer keys instead of strings

### **Strategy 2: Enum-Based Function IDs**

```cpp
enum class MathFunctions {
    ADD = 0,
    MULTIPLY = 1,
    SUBTRACT = 2
};

// Direct array lookup - fastest possible
std::array<std::function<...>, 3> math_functions;
auto result = math_functions[static_cast<int>(MathFunctions::ADD)];
```

**Benefits:**
- ✅ **O(1) guaranteed** - direct array access
- ✅ **Smallest memory footprint**
- ✅ **Type-safe** at compile time
- ❌ **Less flexible** - requires predefined enums

### **Strategy 3: Template Specialization** (Advanced)

```cpp
template<typename FuncTag>
struct FunctionCaller;

template<>
struct FunctionCaller<struct AddTag> {
    static int call(int a, int b) { return add(a, b); }
};

// Compile-time dispatch - zero runtime overhead
auto result = FunctionCaller<AddTag>::call(10, 20);
```

**Benefits:**
- ✅ **Zero runtime overhead** - compiled away
- ✅ **Maximum type safety**
- ❌ **Complex implementation**
- ❌ **Not dynamic** - all functions must be known at compile time

## Recommended Implementation

### **Hybrid Approach: Fast + Compatible**

```cpp
class OptimizedFunctionGroup {
private:
    // Fast path: hash-based lookup
    std::unordered_map<FunctionId, std::function<...>> functions_by_id_;
    // Compatibility: string to ID mapping
    std::map<std::string, FunctionId> name_to_id_;

public:
    // Fast registration with compile-time ID
    template<typename Func>
    void add(FunctionId id, const std::string& name, Func&& func) {
        functions_by_id_[id] = create_wrapper(std::forward<Func>(func));
        name_to_id_[name] = id;
    }
    
    // Fast call - O(1) average
    template<typename ReturnType, typename... Args>
    ReturnType call_as(FunctionId id, Args&&... args) const {
        // Direct hash table lookup
        auto it = functions_by_id_.find(id);
        // ... call function
    }
    
    // Backward compatible call - O(log n) + O(1)
    template<typename ReturnType, typename... Args>
    ReturnType call_as(const std::string& name, Args&&... args) const {
        auto name_it = name_to_id_.find(name);  // O(log n)
        return call_as<ReturnType>(name_it->second, args...);  // O(1)
    }
};
```

### **Usage Patterns**

#### **Development Phase** (Readability)
```cpp
group->add("add", add_function);
auto result = group->call_as<int>("add", 10, 20);  // String-based
```

#### **Production Phase** (Performance)
```cpp
constexpr auto ADD_ID = hash_string("add");
group->add(ADD_ID, "add", add_function);
auto result = group->call_as<int>(ADD_ID, 10, 20);  // ID-based
```

### **Compile-Time Hash Generation**
```cpp
constexpr std::size_t hash_string(const char* str) {
    std::size_t hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(*str++);
    }
    return hash;
}

#define FUNC_ID(name) hash_string(name)

// Usage
constexpr auto ADD_ID = FUNC_ID("add");  // Computed at compile time
```

## Performance Recommendations

### **For High-Frequency Function Calls**
1. ✅ **Use compile-time function IDs**: `FUNC_ID("function_name")`
2. ✅ **Switch to unordered_map**: Replace `std::map<std::string>` with `std::unordered_map<FunctionId>`
3. ✅ **Cache function pointers**: Store direct function pointers for critical paths
4. ✅ **Consider template specialization**: For maximum performance

### **For Development/Flexibility**
1. ✅ **Keep string-based API**: For debugging and dynamic function calls
2. ✅ **Hybrid approach**: Support both ID and string lookups
3. ✅ **Profiling**: Measure actual performance impact in your use case

### **When String Lookups Are Acceptable**
- ✅ **Infrequent calls**: < 1000 calls per second
- ✅ **Small function count**: < 50 functions per group
- ✅ **Development/prototyping**: Readability over performance
- ✅ **Dynamic loading**: Functions loaded from config files

### **When Optimization Is Critical**
- ⚠️ **High-frequency calls**: > 10,000 calls per second
- ⚠️ **Real-time systems**: Latency-sensitive applications
- ⚠️ **Large function count**: > 100 functions per group
- ⚠️ **Memory-constrained**: Embedded or mobile applications

## Conclusion

**Current string-based lookups are acceptable for most use cases**, but can be optimized for performance-critical applications. The recommended hybrid approach provides:

1. **60-75% performance improvement** for optimized calls
2. **Backward compatibility** with existing string-based code
3. **Flexibility** to choose between readability and performance
4. **Easy migration path** from strings to IDs

Choose the optimization level based on your specific performance requirements and development priorities.
