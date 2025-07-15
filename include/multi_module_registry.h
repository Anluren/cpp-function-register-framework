#pragma once

#include "simple_modern_function_group.h"
#include <string>
#include <map>
#include <vector>
#include <functional>
#include <any>
#include <optional>
#include <memory>
#include <iostream>

/**
 * Multi-Module Function Registry
 * 
 * This enhanced registry supports multiple modules implementing the same function,
 * allowing callers to choose which module's implementation to use.
 */

// Function categories for organization
enum class FunctionCategory {
    MATH,
    STRING,
    UTILITY,
    FILE,
    NETWORK
};

// Standard function identifiers
enum class StandardFunctionId {
    // Math functions
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    SQUARE,
    SQRT,
    POWER,
    ABS,
    
    // String functions
    TO_UPPER,
    TO_LOWER,
    CONCAT,
    LENGTH,
    REVERSE,
    
    // Utility functions
    IS_EVEN,
    FIBONACCI,
    RANDOM_INT,
    SLEEP_MS
};

// Forward declaration of utility function
std::string to_string(StandardFunctionId func_id);

/**
 * Function implementation info
 */
struct FunctionImplementation {
    std::string module_name;
    std::string description;
    std::function<std::any(const std::vector<std::any>&)> function;
    
    FunctionImplementation(const std::string& module, 
                          const std::string& desc,
                          std::function<std::any(const std::vector<std::any>&)> func)
        : module_name(module), description(desc), function(std::move(func)) {}
};

/**
 * Multi-Module Function Registry
 * Supports multiple implementations of the same function
 */
class MultiModuleFunctionRegistry {
private:
    // Map: FunctionId -> Vector of implementations from different modules
    std::map<StandardFunctionId, std::vector<std::unique_ptr<FunctionImplementation>>> function_implementations_;
    
    // Map: ModuleName -> Functions it implements
    std::map<std::string, std::vector<StandardFunctionId>> module_functions_;
    
    static inline std::unique_ptr<MultiModuleFunctionRegistry> instance_;
    
public:
    static MultiModuleFunctionRegistry& instance() {
        if (!instance_) {
            instance_ = std::make_unique<MultiModuleFunctionRegistry>();
        }
        return *instance_;
    }
    
    // Register a function implementation from a specific module
    template<typename Func>
    void register_function(const std::string& module_name,
                          StandardFunctionId func_id,
                          const std::string& description,
                          Func&& func) {
        // Create wrapper function
        auto wrapper = [func = std::forward<Func>(func)](const std::vector<std::any>& args) -> std::any {
            // Handle different argument counts
            if (args.empty()) {
                if constexpr (std::is_invocable_v<Func>) {
                    if constexpr (std::is_void_v<std::invoke_result_t<Func>>) {
                        func();
                        return std::any{};
                    } else {
                        return std::any(func());
                    }
                }
            } else if (args.size() == 1) {
                // Try common single argument types
                if constexpr (std::is_invocable_v<Func, int>) {
                    try {
                        auto val = std::any_cast<int>(args[0]);
                        if constexpr (std::is_void_v<std::invoke_result_t<Func, int>>) {
                            func(val);
                            return std::any{};
                        } else {
                            return std::any(func(val));
                        }
                    } catch (const std::bad_any_cast&) {}
                }
                if constexpr (std::is_invocable_v<Func, const std::string&>) {
                    try {
                        auto val = std::any_cast<std::string>(args[0]);
                        if constexpr (std::is_void_v<std::invoke_result_t<Func, const std::string&>>) {
                            func(val);
                            return std::any{};
                        } else {
                            return std::any(func(val));
                        }
                    } catch (const std::bad_any_cast&) {}
                }
            } else if (args.size() == 2) {
                // Try common two argument types
                if constexpr (std::is_invocable_v<Func, int, int>) {
                    try {
                        auto val1 = std::any_cast<int>(args[0]);
                        auto val2 = std::any_cast<int>(args[1]);
                        if constexpr (std::is_void_v<std::invoke_result_t<Func, int, int>>) {
                            func(val1, val2);
                            return std::any{};
                        } else {
                            return std::any(func(val1, val2));
                        }
                    } catch (const std::bad_any_cast&) {}
                }
            }
            throw std::runtime_error("Cannot call function with provided argument types");
        };
        
        // Create implementation entry
        auto impl = std::make_unique<FunctionImplementation>(module_name, description, wrapper);
        
        // Add to function implementations
        function_implementations_[func_id].push_back(std::move(impl));
        
        // Track module functions
        module_functions_[module_name].push_back(func_id);
        
        std::cout << "Registered " << to_string(func_id) << " from module: " << module_name << std::endl;
    }
    
    // Get all modules that implement a specific function
    std::vector<std::string> get_function_providers(StandardFunctionId func_id) const {
        std::vector<std::string> providers;
        auto it = function_implementations_.find(func_id);
        if (it != function_implementations_.end()) {
            for (const auto& impl : it->second) {
                providers.push_back(impl->module_name);
            }
        }
        return providers;
    }
    
    // Get all functions implemented by a specific module
    std::vector<StandardFunctionId> get_module_functions(const std::string& module_name) const {
        auto it = module_functions_.find(module_name);
        return it != module_functions_.end() ? it->second : std::vector<StandardFunctionId>{};
    }
    
    // Call function from a specific module
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> call_function(const std::string& module_name,
                                           StandardFunctionId func_id,
                                           Args&&... args) {
        auto it = function_implementations_.find(func_id);
        if (it == function_implementations_.end()) {
            return std::nullopt; // Function not implemented by any module
        }
        
        // Find the specific module's implementation
        for (const auto& impl : it->second) {
            if (impl->module_name == module_name) {
                try {
                    std::vector<std::any> arg_vector;
                    if constexpr (sizeof...(args) > 0) {
                        (arg_vector.emplace_back(std::forward<Args>(args)), ...);
                    }
                    
                    auto result = impl->function(arg_vector);
                    if constexpr (std::is_void_v<ReturnType>) {
                        return std::nullopt; // Success indicator for void functions
                    } else {
                        return std::any_cast<ReturnType>(result);
                    }
                } catch (...) {
                    return std::nullopt;
                }
            }
        }
        
        return std::nullopt; // Module doesn't implement this function
    }
    
    // Call function from any available module (first found)
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> call_function_any(StandardFunctionId func_id, Args&&... args) {
        auto it = function_implementations_.find(func_id);
        if (it == function_implementations_.end() || it->second.empty()) {
            return std::nullopt;
        }
        
        // Try each implementation until one succeeds
        for (const auto& impl : it->second) {
            try {
                std::vector<std::any> arg_vector;
                if constexpr (sizeof...(args) > 0) {
                    (arg_vector.emplace_back(std::forward<Args>(args)), ...);
                }
                
                auto result = impl->function(arg_vector);
                if constexpr (std::is_void_v<ReturnType>) {
                    return std::nullopt; // Success
                } else {
                    return std::any_cast<ReturnType>(result);
                }
            } catch (...) {
                continue; // Try next implementation
            }
        }
        
        return std::nullopt;
    }
    
    // Get list of all registered modules
    std::vector<std::string> get_all_modules() const {
        std::vector<std::string> modules;
        for (const auto& [module_name, functions] : module_functions_) {
            modules.push_back(module_name);
        }
        return modules;
    }
    
    // Show implementation overview
    void show_implementation_overview() const {
        std::cout << "\n=== Multi-Module Function Registry Overview ===" << std::endl;
        
        // Show functions and their providers
        for (const auto& [func_id, implementations] : function_implementations_) {
            std::cout << to_string(func_id) << " (" << implementations.size() << " implementations):" << std::endl;
            for (const auto& impl : implementations) {
                std::cout << "  - " << impl->module_name << ": " << impl->description << std::endl;
            }
        }
        
        std::cout << "\n=== Modules and their functions ===" << std::endl;
        for (const auto& [module_name, functions] : module_functions_) {
            std::cout << module_name << " (" << functions.size() << " functions):" << std::endl;
            for (const auto& func_id : functions) {
                std::cout << "  - " << to_string(func_id) << std::endl;
            }
        }
    }
    
    // Check if specific module implements a function
    bool module_has_function(const std::string& module_name, StandardFunctionId func_id) const {
        auto it = function_implementations_.find(func_id);
        if (it == function_implementations_.end()) return false;
        
        for (const auto& impl : it->second) {
            if (impl->module_name == module_name) {
                return true;
            }
        }
        return false;
    }
};

// Utility function to convert function ID to string
inline std::string to_string(StandardFunctionId func_id) {
    switch (func_id) {
        case StandardFunctionId::ADD: return "ADD";
        case StandardFunctionId::SUBTRACT: return "SUBTRACT";
        case StandardFunctionId::MULTIPLY: return "MULTIPLY";
        case StandardFunctionId::DIVIDE: return "DIVIDE";
        case StandardFunctionId::SQUARE: return "SQUARE";
        case StandardFunctionId::SQRT: return "SQRT";
        case StandardFunctionId::POWER: return "POWER";
        case StandardFunctionId::ABS: return "ABS";
        case StandardFunctionId::TO_UPPER: return "TO_UPPER";
        case StandardFunctionId::TO_LOWER: return "TO_LOWER";
        case StandardFunctionId::CONCAT: return "CONCAT";
        case StandardFunctionId::LENGTH: return "LENGTH";
        case StandardFunctionId::REVERSE: return "REVERSE";
        case StandardFunctionId::IS_EVEN: return "IS_EVEN";
        case StandardFunctionId::FIBONACCI: return "FIBONACCI";
        case StandardFunctionId::RANDOM_INT: return "RANDOM_INT";
        case StandardFunctionId::SLEEP_MS: return "SLEEP_MS";
        default: return "UNKNOWN";
    }
}
