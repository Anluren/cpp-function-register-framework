#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <array>
#include <string_view>
#include <stdexcept>

namespace Optimized {

/**
 * High-performance function categories using enum class
 */
enum class FunctionCategory : uint8_t {
    MATH = 0,
    STRING = 1,
    UTILITY = 2
};

/**
 * Compile-time function ID generation
 */
constexpr uint32_t hash_string_view(std::string_view str) {
    uint32_t hash = 2166136261u;
    for (char c : str) {
        hash ^= static_cast<uint32_t>(c);
        hash *= 16777619u;
    }
    return hash;
}

/**
 * High-performance function storage using function pointers
 * Minimizes overhead compared to std::function
 */
template<typename R, typename... Args>
struct FastFunction {
    using FuncPtr = R(*)(Args...);
    FuncPtr ptr;
    uint32_t id;
    std::string_view name;
    
    constexpr FastFunction(std::string_view n, FuncPtr p) 
        : ptr(p), id(hash_string_view(n)), name(n) {}
    
    R call(Args... args) const {
        return ptr(args...);
    }
    
    R operator()(Args... args) const {
        return ptr(args...);
    }
};

/**
 * Optimized function group with minimal overhead
 * Uses unordered_map for O(1) lookup and avoids virtual calls
 */
class OptimizedFunctionGroup {
private:
    std::string groupName;
    FunctionCategory category;
    std::unordered_map<uint32_t, void*> functionPtrs;
    std::unordered_map<uint32_t, std::string_view> functionNames;
    
public:
    explicit OptimizedFunctionGroup(std::string_view name, FunctionCategory cat) 
        : groupName(name), category(cat) {
        // Reserve space to avoid rehashing
        functionPtrs.reserve(64);
        functionNames.reserve(64);
    }
    
    // Register function with compile-time ID generation
    template<typename R, typename... Args>
    void registerFunction(std::string_view name, R(*func)(Args...)) {
        auto id = hash_string_view(name);
        functionPtrs[id] = reinterpret_cast<void*>(func);
        functionNames[id] = name;
    }
    
    // Fast function lookup using compile-time hash
    template<typename R, typename... Args>
    R callFunction(std::string_view name, Args... args) const {
        auto id = hash_string_view(name);
        auto it = functionPtrs.find(id);
        if (it != functionPtrs.end()) {
            auto func = reinterpret_cast<R(*)(Args...)>(it->second);
            return func(args...);
        }
        throw std::runtime_error("Function not found");
    }
    
    // Get function pointer directly
    template<typename R, typename... Args>
    R(*getFunction(std::string_view name))(Args...) {
        auto id = hash_string_view(name);
        auto it = functionPtrs.find(id);
        return (it != functionPtrs.end()) 
            ? reinterpret_cast<R(*)(Args...)>(it->second) 
            : nullptr;
    }
    
    // Check if function exists (very fast)
    bool hasFunction(std::string_view name) const {
        return functionPtrs.find(hash_string_view(name)) != functionPtrs.end();
    }
    
    // Get all function names
    std::vector<std::string> getFunctionNames() const {
        std::vector<std::string> names;
        names.reserve(functionNames.size());
        for (const auto& pair : functionNames) {
            names.emplace_back(pair.second);
        }
        return names;
    }
    
    // Get group info
    const std::string& getName() const { return groupName; }
    FunctionCategory getCategory() const { return category; }
    size_t getFunctionCount() const { return functionPtrs.size(); }
};

/**
 * Performance-optimized function types
 */
using IntFunction = int(*)();
using IntIntFunction = int(*)(int);
using IntIntIntFunction = int(*)(int, int);
using DoubleFunction = double(*)();
using DoubleDoubleFunction = double(*)(double);
using DoubleDoubleDoubleFunction = double(*)(double, double);
using StringFunction = const char*(*)();
using StringStringFunction = const char*(*)(const char*);

} // namespace Optimized
