#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <any>
#include <optional>
#include <memory>
#include <type_traits>

/**
 * Predefined Function Interface System
 * 
 * This system allows you to define a fixed set of function signatures
 * that modules can implement. Each module registers only the functions
 * it supports from the predefined interface.
 */

// Forward declarations
class IFunctionProvider;

/**
 * Enum defining all possible functions in the system
 * Add new functions here when extending the interface
 */
enum class StandardFunction {
    // Math operations
    ADD_INT,
    MULTIPLY_DOUBLE,
    SQUARE_INT,
    POWER_DOUBLE,
    
    // String operations
    TO_UPPER,
    TO_LOWER,
    CONCAT,
    GET_LENGTH,
    
    // Utility operations
    IS_EVEN,
    FIBONACCI,
    RANDOM_INT,
    SLEEP_MS,
    
    // File operations
    READ_FILE,
    WRITE_FILE,
    FILE_EXISTS,
    
    // Network operations
    HTTP_GET,
    PING_HOST,
    
    // Custom operations
    CUSTOM_OPERATION_1,
    CUSTOM_OPERATION_2
};

/**
 * Function signature definitions
 * This defines the exact signature for each standard function
 */
struct FunctionSignatures {
    // Math operations
    using AddInt = std::function<int(int, int)>;
    using MultiplyDouble = std::function<double(double, double)>;
    using SquareInt = std::function<int(int)>;
    using PowerDouble = std::function<double(double, double)>;
    
    // String operations
    using ToUpper = std::function<std::string(const std::string&)>;
    using ToLower = std::function<std::string(const std::string&)>;
    using Concat = std::function<std::string(const std::string&, const std::string&)>;
    using GetLength = std::function<int(const std::string&)>;
    
    // Utility operations
    using IsEven = std::function<bool(int)>;
    using Fibonacci = std::function<int(int)>;
    using RandomInt = std::function<int()>;
    using SleepMs = std::function<void(int)>;
    
    // File operations
    using ReadFile = std::function<std::string(const std::string&)>;
    using WriteFile = std::function<bool(const std::string&, const std::string&)>;
    using FileExists = std::function<bool(const std::string&)>;
    
    // Network operations
    using HttpGet = std::function<std::string(const std::string&)>;
    using PingHost = std::function<bool(const std::string&)>;
    
    // Custom operations
    using CustomOperation1 = std::function<std::any(const std::any&)>;
    using CustomOperation2 = std::function<std::any(const std::any&, const std::any&)>;
};

/**
 * Type-erased function wrapper for standard functions
 */
class StandardFunctionWrapper {
private:
    std::any function_;
    StandardFunction type_;
    
public:
    template<typename Func>
    StandardFunctionWrapper(StandardFunction type, Func&& func) 
        : function_(std::forward<Func>(func)), type_(type) {}
    
    StandardFunction type() const { return type_; }
    
    template<typename ReturnType, typename... Args>
    ReturnType call(Args&&... args) const {
        switch (type_) {
            case StandardFunction::ADD_INT:
                return std::any_cast<FunctionSignatures::AddInt>(function_)(std::forward<Args>(args)...);
            case StandardFunction::MULTIPLY_DOUBLE:
                return std::any_cast<FunctionSignatures::MultiplyDouble>(function_)(std::forward<Args>(args)...);
            case StandardFunction::TO_UPPER:
                return std::any_cast<FunctionSignatures::ToUpper>(function_)(std::forward<Args>(args)...);
            // ... add other cases as needed
            default:
                throw std::runtime_error("Function type not implemented in call");
        }
    }
    
    // Type-safe call methods for each function type
    int call_add_int(int a, int b) const {
        return std::any_cast<FunctionSignatures::AddInt>(function_)(a, b);
    }
    
    double call_multiply_double(double a, double b) const {
        return std::any_cast<FunctionSignatures::MultiplyDouble>(function_)(a, b);
    }
    
    std::string call_to_upper(const std::string& str) const {
        return std::any_cast<FunctionSignatures::ToUpper>(function_)(str);
    }
    
    // Add more type-safe call methods as needed...
};

/**
 * Registry for standard functions
 * Modules register subsets of the predefined functions
 */
class StandardFunctionRegistry {
private:
    std::map<StandardFunction, std::unique_ptr<StandardFunctionWrapper>> functions_;
    std::map<std::string, std::vector<StandardFunction>> modules_;
    static inline std::unique_ptr<StandardFunctionRegistry> instance_;
    
public:
    static StandardFunctionRegistry& instance() {
        if (!instance_) {
            instance_ = std::make_unique<StandardFunctionRegistry>();
        }
        return *instance_;
    }
    
    // Register a function from a specific module
    template<typename Func>
    void register_function(const std::string& module_name, StandardFunction func_type, Func&& func) {
        functions_[func_type] = std::make_unique<StandardFunctionWrapper>(func_type, std::forward<Func>(func));
        modules_[module_name].push_back(func_type);
    }
    
    // Check if a function is registered
    bool has_function(StandardFunction func_type) const {
        return functions_.find(func_type) != functions_.end();
    }
    
    // Get function wrapper
    StandardFunctionWrapper* get_function(StandardFunction func_type) {
        auto it = functions_.find(func_type);
        return it != functions_.end() ? it->second.get() : nullptr;
    }
    
    // Get all functions registered by a module
    std::vector<StandardFunction> get_module_functions(const std::string& module_name) const {
        auto it = modules_.find(module_name);
        return it != modules_.end() ? it->second : std::vector<StandardFunction>{};
    }
    
    // Get all registered functions
    std::vector<StandardFunction> get_all_functions() const {
        std::vector<StandardFunction> functions;
        for (const auto& [func, wrapper] : functions_) {
            functions.push_back(func);
        }
        return functions;
    }
    
    // Get list of modules
    std::vector<std::string> get_modules() const {
        std::vector<std::string> module_names;
        for (const auto& [name, functions] : modules_) {
            module_names.push_back(name);
        }
        return module_names;
    }
    
    // Type-safe call methods
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> call_function(StandardFunction func_type, Args&&... args) {
        auto* wrapper = get_function(func_type);
        if (!wrapper) {
            return std::nullopt;
        }
        
        try {
            return wrapper->template call<ReturnType>(std::forward<Args>(args)...);
        } catch (...) {
            return std::nullopt;
        }
    }
    
    // Convenience methods for common functions
    std::optional<int> add_int(int a, int b) {
        return call_function<int>(StandardFunction::ADD_INT, a, b);
    }
    
    std::optional<std::string> to_upper(const std::string& str) {
        return call_function<std::string>(StandardFunction::TO_UPPER, str);
    }
    
    std::optional<bool> is_even(int n) {
        return call_function<bool>(StandardFunction::IS_EVEN, n);
    }
};

/**
 * Helper class for modules to register their functions
 */
class ModuleRegistrar {
private:
    std::string module_name_;
    
public:
    explicit ModuleRegistrar(std::string module_name) : module_name_(std::move(module_name)) {}
    
    template<typename Func>
    void register_function(StandardFunction func_type, Func&& func) {
        auto& registry = StandardFunctionRegistry::instance();
        registry.register_function(module_name_, func_type, std::forward<Func>(func));
    }
    
    const std::string& module_name() const { return module_name_; }
};

/**
 * Utility functions to get function names for debugging
 */
inline std::string to_string(StandardFunction func) {
    switch (func) {
        case StandardFunction::ADD_INT: return "ADD_INT";
        case StandardFunction::MULTIPLY_DOUBLE: return "MULTIPLY_DOUBLE";
        case StandardFunction::SQUARE_INT: return "SQUARE_INT";
        case StandardFunction::POWER_DOUBLE: return "POWER_DOUBLE";
        case StandardFunction::TO_UPPER: return "TO_UPPER";
        case StandardFunction::TO_LOWER: return "TO_LOWER";
        case StandardFunction::CONCAT: return "CONCAT";
        case StandardFunction::GET_LENGTH: return "GET_LENGTH";
        case StandardFunction::IS_EVEN: return "IS_EVEN";
        case StandardFunction::FIBONACCI: return "FIBONACCI";
        case StandardFunction::RANDOM_INT: return "RANDOM_INT";
        case StandardFunction::SLEEP_MS: return "SLEEP_MS";
        case StandardFunction::READ_FILE: return "READ_FILE";
        case StandardFunction::WRITE_FILE: return "WRITE_FILE";
        case StandardFunction::FILE_EXISTS: return "FILE_EXISTS";
        case StandardFunction::HTTP_GET: return "HTTP_GET";
        case StandardFunction::PING_HOST: return "PING_HOST";
        case StandardFunction::CUSTOM_OPERATION_1: return "CUSTOM_OPERATION_1";
        case StandardFunction::CUSTOM_OPERATION_2: return "CUSTOM_OPERATION_2";
        default: return "UNKNOWN";
    }
}
