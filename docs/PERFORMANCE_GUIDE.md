# Performance Optimization Summary

## Quick Reference: When to Use Each Approach

### 🔍 **String-Based Lookup** (Default Choice)
```cpp
group->call_as<int>("function_name", args...);
```
**Performance**: ~234-249 ns per call  
**Use when**: Development, debugging, configuration-driven calls, < 10K calls/sec

### ⚡ **ID-Based Lookup** (Optimized)
```cpp
constexpr auto FUNC_ID = hash_string("function_name");
group->call_as<int>(FUNC_ID, args...);
```
**Performance**: ~224 ns per call  
**Use when**: Hot paths, > 10K calls/sec, performance-critical code

### 🚀 **Direct Function Calls** (Maximum Performance)
```cpp
int result = function_name(args...);
```
**Performance**: ~49 ns per call  
**Use when**: No dynamic dispatch needed, compile-time function set

## Performance Impact Calculator

| Calls/Second | String Overhead | ID Overhead | Recommendation |
|-------------|----------------|-------------|----------------|
| 1,000 | 0.2 ms | 0.2 ms | Any approach |
| 10,000 | 2.3 ms | 2.2 ms | String OK |
| 100,000 | 23 ms | 22 ms | Consider ID |
| 1,000,000 | 230 ms | 220 ms | Use ID or direct |

## Migration Strategy

### Phase 1: Development (String-based)
```cpp
// Easy to read and debug
auto result = group->call_as<int>("calculate", input);
```

### Phase 2: Profiling
```cpp
// Add timing measurements
auto start = std::chrono::high_resolution_clock::now();
auto result = group->call_as<int>("calculate", input);
auto end = std::chrono::high_resolution_clock::now();
// Log if this function is called frequently
```

### Phase 3: Optimization (Hot paths only)
```cpp
// Migrate only performance-critical functions
constexpr auto CALC_ID = FUNC_ID("calculate");
auto result = group->call_as<int>(CALC_ID, input);
```

### Phase 4: Hybrid Approach
```cpp
// Keep both for different use cases
class OptimizedGroup {
    // Fast path for hot functions
    T call_fast(FunctionId id, Args... args);
    
    // Readable path for debugging
    T call_debug(const std::string& name, Args... args);
};
```

## Key Takeaways

1. **The overhead difference is modest** (~4.6x vs ~4.8x) - function wrapping is the main cost
2. **String lookup is fine for most use cases** - optimize only when needed
3. **Measure before optimizing** - profile to find actual bottlenecks
4. **Readability matters** - string names are easier to debug and maintain
5. **Use hybrid approach** - fast path for hot code, strings for everything else

## Example Decision Tree

```
Is this function called > 10,000 times/second?
├─ No  → Use string-based lookup
└─ Yes → Is this a performance bottleneck?
    ├─ No  → Use string-based lookup
    └─ Yes → Is the function lightweight?
        ├─ No  → String lookup overhead is negligible
        └─ Yes → Use ID-based lookup or direct calls
```

**Bottom line**: Start with string-based lookup, profile your application, then optimize only the hot paths that actually matter to your use case.
