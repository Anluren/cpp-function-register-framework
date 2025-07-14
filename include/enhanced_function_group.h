#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <typeinfo>
#include <stdexcept>
#include <memory>

/**
 * Enum to identify different function groups
 * Add your custom function group types here
 */
enum FunctionGroupType {
    MATH_FUNCTIONS,
    STRING_FUNCTIONS,
    UTILITY_FUNCTIONS,
    CUSTOM_GROUP_1,
    CUSTOM_GROUP_2
};

/**
 * Enhanced function signature information
 */
struct FunctionSignature {
    std::string name;
    std::vector<std::string> parameterTypes;
    std::string returnType;
    int parameterCount;
    
    FunctionSignature(const std::string& funcName, 
                     const std::vector<std::string>& params,
                     const std::string& retType)
        : name(funcName), parameterTypes(params), returnType(retType), 
          parameterCount(static_cast<int>(params.size())) {}
};

/**
 * Function call result with type information
 */
class FunctionResult {
private:
    void* data;
    std::string type;
    bool valid;
    
public:
    FunctionResult() : data(nullptr), valid(false) {}
    FunctionResult(void* ptr, const std::string& typeName) 
        : data(ptr), type(typeName), valid(true) {}
    
    template<typename T>
    T getValue() const {
        if (!valid) {
            throw std::runtime_error("Invalid function result");
        }
        return *static_cast<T*>(data);
    }
    
    template<typename T>
    bool tryGetValue(T& output) const {
        if (!valid || !data) return false;
        try {
            output = *static_cast<T*>(data);
            return true;
        } catch (...) {
            return false;
        }
    }
    
    bool isValid() const { return valid; }
    std::string getType() const { return type; }
};

/**
 * Enhanced base class for function groups
 * Provides better type safety and introspection
 */
class EnhancedFunctionGroup {
public:
    virtual ~EnhancedFunctionGroup() {}
    
    // Basic information
    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;
    virtual std::string getVersion() const = 0;
    
    // Function introspection
    virtual std::vector<std::string> getFunctionNames() const = 0;
    virtual std::vector<FunctionSignature> getFunctionSignatures() const = 0;
    virtual bool hasFunction(const std::string& functionName) const = 0;
    virtual FunctionSignature getFunctionSignature(const std::string& functionName) const = 0;
    
    // Enhanced function calling with type safety
    virtual FunctionResult callFunction(const std::string& functionName) const = 0;
    virtual FunctionResult callFunction(const std::string& functionName, 
                                      const std::vector<void*>& args) const = 0;
    
    // Template-based type-safe calling
    template<typename ReturnType>
    ReturnType call(const std::string& functionName) const {
        FunctionResult result = callFunction(functionName);
        return result.getValue<ReturnType>();
    }
    
    template<typename ReturnType, typename... Args>
    ReturnType call(const std::string& functionName, Args&&... args) const {
        std::vector<void*> argPtrs;
        addArgs(argPtrs, std::forward<Args>(args)...);
        FunctionResult result = callFunction(functionName, argPtrs);
        return result.getValue<ReturnType>();
    }
    
private:
    template<typename T>
    void addArgs(std::vector<void*>& argPtrs, T&& arg) const {
        argPtrs.push_back(const_cast<void*>(static_cast<const void*>(&arg)));
    }
    
    template<typename T, typename... Rest>
    void addArgs(std::vector<void*>& argPtrs, T&& first, Rest&&... rest) const {
        addArgs(argPtrs, std::forward<T>(first));
        addArgs(argPtrs, std::forward<Rest>(rest)...);
    }
};

/**
 * Modern function wrapper using std::function
 */
class ModernFunctionWrapper {
private:
    std::function<void*()> func0;
    std::function<void*(const std::vector<void*>&)> funcN;
    FunctionSignature signature;
    bool isVariadic;
    
public:
    template<typename ReturnType>
    ModernFunctionWrapper(const std::string& name, std::function<ReturnType()> f)
        : signature(name, {}, typeid(ReturnType).name()), isVariadic(false) {
        func0 = [f]() -> void* {
            static ReturnType result = f();
            return &result;
        };
    }
    
    template<typename ReturnType, typename... Args>
    ModernFunctionWrapper(const std::string& name, std::function<ReturnType(Args...)> f)
        : signature(name, getTypeNames<Args...>(), typeid(ReturnType).name()), isVariadic(true) {
        funcN = [f](const std::vector<void*>& args) -> void* {
            if (args.size() != sizeof...(Args)) {
                throw std::runtime_error("Argument count mismatch");
            }
            static ReturnType result = callWithArgs(f, args, std::index_sequence_for<Args...>{});
            return &result;
        };
    }
    
    FunctionResult call() const {
        if (!isVariadic) {
            return FunctionResult(func0(), signature.returnType);
        }
        throw std::runtime_error("Function requires arguments");
    }
    
    FunctionResult call(const std::vector<void*>& args) const {
        if (isVariadic) {
            return FunctionResult(funcN(args), signature.returnType);
        }
        if (args.empty()) {
            return call();
        }
        throw std::runtime_error("Function does not accept arguments");
    }
    
    const FunctionSignature& getSignature() const { return signature; }
    
private:
    template<typename... Types>
    std::vector<std::string> getTypeNames() const {
        return {typeid(Types).name()...};
    }
    
    template<typename Func, std::size_t... Is>
    auto callWithArgs(Func f, const std::vector<void*>& args, std::index_sequence<Is...>) const 
        -> decltype(f(*static_cast<std::tuple_element_t<Is, std::tuple<Args...>>*>(args[Is])...)) {
        return f(*static_cast<std::tuple_element_t<Is, std::tuple<Args...>>*>(args[Is])...);
    }
};

/**
 * Enhanced concrete implementation with modern C++ features
 */
class ModernFunctionGroup : public EnhancedFunctionGroup {
private:
    std::string groupName;
    std::string description;
    std::string version;
    std::map<std::string, std::shared_ptr<ModernFunctionWrapper>> functions;
    
public:
    ModernFunctionGroup(const std::string& name, 
                       const std::string& desc = "", 
                       const std::string& ver = "1.0.0")
        : groupName(name), description(desc), version(ver) {}
    
    // Basic information
    std::string getName() const override { return groupName; }
    std::string getDescription() const override { return description; }
    std::string getVersion() const override { return version; }
    
    // Function registration with automatic type deduction
    template<typename ReturnType>
    void registerFunction(const std::string& name, std::function<ReturnType()> func) {
        functions[name] = std::make_shared<ModernFunctionWrapper>(name, func);
    }
    
    template<typename ReturnType, typename... Args>
    void registerFunction(const std::string& name, std::function<ReturnType(Args...)> func) {
        functions[name] = std::make_shared<ModernFunctionWrapper>(name, func);
    }
    
    // Convenience registration for function pointers
    template<typename ReturnType>
    void registerFunction(const std::string& name, ReturnType(*func)()) {
        registerFunction(name, std::function<ReturnType()>(func));
    }
    
    template<typename ReturnType, typename... Args>
    void registerFunction(const std::string& name, ReturnType(*func)(Args...)) {
        registerFunction(name, std::function<ReturnType(Args...)>(func));
    }
    
    // Function introspection
    std::vector<std::string> getFunctionNames() const override {
        std::vector<std::string> names;
        for (const auto& pair : functions) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    std::vector<FunctionSignature> getFunctionSignatures() const override {
        std::vector<FunctionSignature> signatures;
        for (const auto& pair : functions) {
            signatures.push_back(pair.second->getSignature());
        }
        return signatures;
    }
    
    bool hasFunction(const std::string& functionName) const override {
        return functions.find(functionName) != functions.end();
    }
    
    FunctionSignature getFunctionSignature(const std::string& functionName) const override {
        auto it = functions.find(functionName);
        if (it != functions.end()) {
            return it->second->getSignature();
        }
        throw std::runtime_error("Function not found: " + functionName);
    }
    
    // Enhanced function calling
    FunctionResult callFunction(const std::string& functionName) const override {
        auto it = functions.find(functionName);
        if (it != functions.end()) {
            return it->second->call();
        }
        return FunctionResult();
    }
    
    FunctionResult callFunction(const std::string& functionName, 
                              const std::vector<void*>& args) const override {
        auto it = functions.find(functionName);
        if (it != functions.end()) {
            return it->second->call(args);
        }
        return FunctionResult();
    }
};
