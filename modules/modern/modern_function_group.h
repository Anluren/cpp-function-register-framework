#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <typeinfo>
#include <type_traits>
#include <future>

namespace Modern {

/**
 * Function categories for modern module
 */
enum class FunctionCategory {
    MATH,
    STRING,
    UTILITY,
    LAMBDA,
    TEMPLATE
};

/**
 * Type-safe function wrapper using std::function
 */
template<typename Signature>
class TypedFunction;

template<typename R, typename... Args>
class TypedFunction<R(Args...)> {
private:
    std::function<R(Args...)> func;
    std::string name;
    std::string signature;

public:
    TypedFunction(const std::string& n, std::function<R(Args...)> f) 
        : func(std::move(f)), name(n), signature(typeid(R(Args...)).name()) {}
    
    R operator()(Args... args) const {
        return func(args...);
    }
    
    R call(Args... args) const {
        return func(args...);
    }
    
    const std::string& getName() const { return name; }
    const std::string& getSignature() const { return signature; }
    bool isValid() const { return static_cast<bool>(func); }
};

/**
 * Modern function group with type safety and smart pointers
 */
class ModernFunctionGroup {
private:
    std::string groupName;
    FunctionCategory category;
    std::map<std::string, std::shared_ptr<void>> functions;
    std::map<std::string, std::string> signatures;

public:
    ModernFunctionGroup(const std::string& name, FunctionCategory cat) 
        : groupName(name), category(cat) {}
    
    virtual ~ModernFunctionGroup() = default;
    
    // Register a typed function
    template<typename Signature>
    void registerFunction(const std::string& name, std::function<Signature> func) {
        auto typedFunc = std::make_shared<TypedFunction<Signature>>(name, std::move(func));
        functions[name] = std::static_pointer_cast<void>(typedFunc);
        signatures[name] = typeid(Signature).name();
    }
    
    // Register a lambda or function pointer - explicit signature version
    template<typename Signature, typename F>
    void registerLambda(const std::string& name, F&& func) {
        auto stdFunc = std::function<Signature>(std::forward<F>(func));
        registerFunction<Signature>(name, stdFunc);
    }
    
    // Get a typed function
    template<typename Signature>
    std::shared_ptr<TypedFunction<Signature>> getFunction(const std::string& name) const {
        auto it = functions.find(name);
        if (it != functions.end()) {
            return std::static_pointer_cast<TypedFunction<Signature>>(it->second);
        }
        return nullptr;
    }
    
    // Check if function exists
    bool hasFunction(const std::string& name) const {
        return functions.find(name) != functions.end();
    }
    
    // Get all function names
    std::vector<std::string> getFunctionNames() const {
        std::vector<std::string> names;
        for (const auto& pair : functions) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    // Get function signature
    std::string getFunctionSignature(const std::string& name) const {
        auto it = signatures.find(name);
        return (it != signatures.end()) ? it->second : "";
    }
    
    // Group info
    const std::string& getName() const { return groupName; }
    FunctionCategory getCategory() const { return category; }
    size_t getFunctionCount() const { return functions.size(); }
};

/**
 * Helper macros for modern function registration
 */
#define MODERN_REGISTER_FUNCTION(group, name, func) \
    (group)->registerFunction<decltype(func)>(name, func)

#define MODERN_REGISTER_LAMBDA(group, name, lambda) \
    (group)->registerLambda(name, lambda)

} // namespace Modern
