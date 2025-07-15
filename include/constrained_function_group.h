#pragma once

#include "simple_modern_function_group.h"
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <memory>
#include <iostream>

/**
 * Template-Based Function Set System
 * 
 * This approach uses compile-time templates to define function interfaces
 * and allows modules to implement subsets of those interfaces.
 */

/**
 * Function interface traits - define what functions are available
 */
template<typename FunctionSet>
struct FunctionInterface;

/**
 * Math Function Set - defines all possible math functions
 */
struct MathFunctionSet {};

template<>
struct FunctionInterface<MathFunctionSet> {
    // Define function signatures as static members
    using AddFunc = std::function<int(int, int)>;
    using MultiplyFunc = std::function<double(double, double)>;
    using SquareFunc = std::function<int(int)>;
    using PowerFunc = std::function<double(double, double)>;
    using SqrtFunc = std::function<double(double)>;
    
    // Function name constants
    static constexpr const char* ADD = "add";
    static constexpr const char* MULTIPLY = "multiply"; 
    static constexpr const char* SQUARE = "square";
    static constexpr const char* POWER = "power";
    static constexpr const char* SQRT = "sqrt";
};

/**
 * String Function Set - defines all possible string functions
 */
struct StringFunctionSet {};

template<>
struct FunctionInterface<StringFunctionSet> {
    using ToUpperFunc = std::function<std::string(const std::string&)>;
    using ToLowerFunc = std::function<std::string(const std::string&)>;
    using ConcatFunc = std::function<std::string(const std::string&, const std::string&)>;
    using LengthFunc = std::function<int(const std::string&)>;
    using ReverseFunc = std::function<std::string(const std::string&)>;
    
    static constexpr const char* TO_UPPER = "to_upper";
    static constexpr const char* TO_LOWER = "to_lower";
    static constexpr const char* CONCAT = "concat";
    static constexpr const char* LENGTH = "length";
    static constexpr const char* REVERSE = "reverse";
};

/**
 * Utility Function Set - defines all possible utility functions
 */
struct UtilityFunctionSet {};

template<>
struct FunctionInterface<UtilityFunctionSet> {
    using IsEvenFunc = std::function<bool(int)>;
    using FibonacciFunc = std::function<int(int)>;
    using RandomFunc = std::function<int()>;
    using SleepFunc = std::function<void(int)>;
    using ClampFunc = std::function<int(int, int, int)>;
    
    static constexpr const char* IS_EVEN = "is_even";
    static constexpr const char* FIBONACCI = "fibonacci";
    static constexpr const char* RANDOM = "random";
    static constexpr const char* SLEEP = "sleep";
    static constexpr const char* CLAMP = "clamp";
};

/**
 * Constrained Function Group - only allows functions from a specific interface
 */
template<typename FunctionSet>
class ConstrainedFunctionGroup : public SimpleFunctionGroup {
private:
    std::vector<std::string> implemented_functions_;
    std::string module_name_;
    
public:
    explicit ConstrainedFunctionGroup(const std::string& module_name) 
        : SimpleFunctionGroup(module_name + " (" + typeid(FunctionSet).name() + ")"),
          module_name_(module_name) {}
    
    // Type-safe function registration - only allows functions from the interface
    template<typename Func>
    void register_function(const std::string& function_name, Func&& func) {
        // This would ideally have compile-time checking, but for simplicity
        // we'll do runtime validation
        add(function_name, std::forward<Func>(func));
        implemented_functions_.push_back(function_name);
        
        std::cout << "Module '" << module_name_ << "' registered function: " 
                  << function_name << std::endl;
    }
    
    // Get list of functions this module implements
    const std::vector<std::string>& implemented_functions() const {
        return implemented_functions_;
    }
    
    const std::string& module_name() const {
        return module_name_;
    }
    
    // Convenience methods for common function types
    template<typename... Args>
    auto call_add(Args&&... args) -> decltype(this->call_as<int>("add", std::forward<Args>(args)...)) {
        return this->call_as<int>("add", std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    auto call_multiply(Args&&... args) -> decltype(this->call_as<double>("multiply", std::forward<Args>(args)...)) {
        return this->call_as<double>("multiply", std::forward<Args>(args)...);
    }
    
    template<typename... Args>
    auto call_to_upper(Args&&... args) -> decltype(this->call_as<std::string>("to_upper", std::forward<Args>(args)...)) {
        return this->call_as<std::string>("to_upper", std::forward<Args>(args)...);
    }
};

/**
 * Registry for constrained function groups
 */
template<typename FunctionSet>
class ConstrainedFunctionRegistry {
private:
    std::map<std::string, std::unique_ptr<ConstrainedFunctionGroup<FunctionSet>>> modules_;
    static inline std::unique_ptr<ConstrainedFunctionRegistry<FunctionSet>> instance_;
    
public:
    static ConstrainedFunctionRegistry<FunctionSet>& instance() {
        if (!instance_) {
            instance_ = std::make_unique<ConstrainedFunctionRegistry<FunctionSet>>();
        }
        return *instance_;
    }
    
    void register_module(const std::string& module_name, 
                        std::unique_ptr<ConstrainedFunctionGroup<FunctionSet>> module) {
        modules_[module_name] = std::move(module);
        std::cout << "Registered module: " << module_name << std::endl;
    }
    
    ConstrainedFunctionGroup<FunctionSet>* get_module(const std::string& module_name) {
        auto it = modules_.find(module_name);
        return it != modules_.end() ? it->second.get() : nullptr;
    }
    
    std::vector<std::string> get_module_names() const {
        std::vector<std::string> names;
        for (const auto& [name, module] : modules_) {
            names.push_back(name);
        }
        return names;
    }
    
    // Find which module implements a specific function
    ConstrainedFunctionGroup<FunctionSet>* find_function_provider(const std::string& function_name) {
        for (const auto& [name, module] : modules_) {
            if (module->has_function(function_name)) {
                return module.get();
            }
        }
        return nullptr;
    }
    
    // Call function from any module that implements it
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> call_function(const std::string& function_name, Args&&... args) {
        auto* provider = find_function_provider(function_name);
        if (!provider) {
            return std::nullopt;
        }
        
        try {
            return provider->template call_as<ReturnType>(function_name, std::forward<Args>(args)...);
        } catch (...) {
            return std::nullopt;
        }
    }
    
    // Get summary of all implemented functions
    std::map<std::string, std::vector<std::string>> get_implementation_summary() const {
        std::map<std::string, std::vector<std::string>> summary;
        for (const auto& [name, module] : modules_) {
            summary[name] = module->implemented_functions();
        }
        return summary;
    }
};

// Type aliases for convenience
using MathRegistry = ConstrainedFunctionRegistry<MathFunctionSet>;
using StringRegistry = ConstrainedFunctionRegistry<StringFunctionSet>;
using UtilityRegistry = ConstrainedFunctionRegistry<UtilityFunctionSet>;

using MathModule = ConstrainedFunctionGroup<MathFunctionSet>;
using StringModule = ConstrainedFunctionGroup<StringFunctionSet>;
using UtilityModule = ConstrainedFunctionGroup<UtilityFunctionSet>;
