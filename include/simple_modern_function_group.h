#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <functional>
#include <any>
#include <optional>
#include <memory>
#include <type_traits>

/**
 * Enum to identify different function groups
 */
enum class FunctionGroupType {
    MATH_FUNCTIONS,
    STRING_FUNCTIONS,
    UTILITY_FUNCTIONS,
    CUSTOM_GROUP_1,
    CUSTOM_GROUP_2
};

/**
 * Simple modern function group using C++17 features
 * Unified interface for function pointers, lambdas, and std::function objects
 */
class SimpleFunctionGroup {
private:
    std::string name_;
    std::map<std::string, std::function<std::any(const std::vector<std::any>&)>> functions_;
    
public:
    explicit SimpleFunctionGroup(std::string name) : name_(std::move(name)) {}
    
    // Get group information
    const std::string& name() const { return name_; }
    
    std::vector<std::string> function_names() const {
        std::vector<std::string> names;
        for (const auto& [name, func] : functions_) {
            names.push_back(name);
        }
        return names;
    }
    
    bool has_function(const std::string& name) const {
        return functions_.find(name) != functions_.end();
    }
    
    // Unified registration for any callable
    template<typename Func>
    void add(const std::string& name, Func&& func) {
        functions_[name] = create_wrapper(std::forward<Func>(func));
    }
    
    // Call function and get result as std::any
    template<typename... Args>
    std::any call(const std::string& name, Args&&... args) const {
        auto it = functions_.find(name);
        if (it == functions_.end()) {
            throw std::runtime_error("Function not found: " + name);
        }
        
        std::vector<std::any> arg_vector;
        if constexpr (sizeof...(args) > 0) {
            (arg_vector.emplace_back(std::forward<Args>(args)), ...);
        }
        
        return it->second(arg_vector);
    }
    
    // Type-safe call with automatic casting
    template<typename ReturnType, typename... Args>
    ReturnType call_as(const std::string& name, Args&&... args) const {
        auto result = call(name, std::forward<Args>(args)...);
        if constexpr (std::is_void_v<ReturnType>) {
            // For void functions, we don't return anything
        } else {
            return std::any_cast<ReturnType>(result);
        }
    }
    
    // Safe call that returns optional
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> try_call(const std::string& name, Args&&... args) const {
        try {
            if constexpr (std::is_void_v<ReturnType>) {
                call(name, std::forward<Args>(args)...);
                return std::nullopt; // void functions return nullopt to indicate success
            } else {
                auto result = call(name, std::forward<Args>(args)...);
                return std::any_cast<ReturnType>(result);
            }
        } catch (...) {
            return std::nullopt;
        }
    }

private:
    // Create wrapper for function with no arguments
    template<typename Func>
    auto create_wrapper(Func&& func) -> std::function<std::any(const std::vector<std::any>&)> {
        return [this, f = std::forward<Func>(func)](const std::vector<std::any>& args) -> std::any {
            // Try to deduce argument count by calling with different signatures
            if (args.empty()) {
                // No arguments
                if constexpr (std::is_invocable_v<Func>) {
                    if constexpr (std::is_void_v<std::invoke_result_t<Func>>) {
                        f();
                        return std::any{};
                    } else {
                        return std::any(f());
                    }
                } else {
                    throw std::runtime_error("Function not callable with 0 arguments");
                }
            } else if (args.size() == 1) {
                // One argument - try common types
                return this->call_with_one_arg(f, args[0]);
            } else if (args.size() == 2) {
                // Two arguments - try common type combinations
                return this->call_with_two_args(f, args[0], args[1]);
            } else {
                throw std::runtime_error("Functions with more than 2 arguments not yet supported");
            }
        };
    }
    
    template<typename Func>
    std::any call_with_one_arg(Func&& func, const std::any& arg) const {
        // Try different single argument types
        try {
            if constexpr (std::is_invocable_v<Func, int>) {
                auto val = std::any_cast<int>(arg);
                if constexpr (std::is_void_v<std::invoke_result_t<Func, int>>) {
                    func(val);
                    return std::any{};
                } else {
                    return std::any(func(val));
                }
            }
        } catch (const std::bad_any_cast&) {}
        
        try {
            if constexpr (std::is_invocable_v<Func, double>) {
                auto val = std::any_cast<double>(arg);
                if constexpr (std::is_void_v<std::invoke_result_t<Func, double>>) {
                    func(val);
                    return std::any{};
                } else {
                    return std::any(func(val));
                }
            }
        } catch (const std::bad_any_cast&) {}
        
        try {
            if constexpr (std::is_invocable_v<Func, std::string>) {
                auto val = std::any_cast<std::string>(arg);
                if constexpr (std::is_void_v<std::invoke_result_t<Func, std::string>>) {
                    func(val);
                    return std::any{};
                } else {
                    return std::any(func(val));
                }
            }
        } catch (const std::bad_any_cast&) {}
        
        try {
            if constexpr (std::is_invocable_v<Func, const std::string&>) {
                auto val = std::any_cast<std::string>(arg);
                if constexpr (std::is_void_v<std::invoke_result_t<Func, const std::string&>>) {
                    func(val);
                    return std::any{};
                } else {
                    return std::any(func(val));
                }
            }
        } catch (const std::bad_any_cast&) {}
        
        throw std::runtime_error("Cannot call function with provided argument type");
    }
    
    template<typename Func>
    std::any call_with_two_args(Func&& func, const std::any& arg1, const std::any& arg2) const {
        // Try different two argument type combinations
        try {
            if constexpr (std::is_invocable_v<Func, int, int>) {
                auto val1 = std::any_cast<int>(arg1);
                auto val2 = std::any_cast<int>(arg2);
                if constexpr (std::is_void_v<std::invoke_result_t<Func, int, int>>) {
                    func(val1, val2);
                    return std::any{};
                } else {
                    return std::any(func(val1, val2));
                }
            }
        } catch (const std::bad_any_cast&) {}
        
        try {
            if constexpr (std::is_invocable_v<Func, double, double>) {
                auto val1 = std::any_cast<double>(arg1);
                auto val2 = std::any_cast<double>(arg2);
                if constexpr (std::is_void_v<std::invoke_result_t<Func, double, double>>) {
                    func(val1, val2);
                    return std::any{};
                } else {
                    return std::any(func(val1, val2));
                }
            }
        } catch (const std::bad_any_cast&) {}
        
        try {
            if constexpr (std::is_invocable_v<Func, std::string, std::string>) {
                auto val1 = std::any_cast<std::string>(arg1);
                auto val2 = std::any_cast<std::string>(arg2);
                if constexpr (std::is_void_v<std::invoke_result_t<Func, std::string, std::string>>) {
                    func(val1, val2);
                    return std::any{};
                } else {
                    return std::any(func(val1, val2));
                }
            }
        } catch (const std::bad_any_cast&) {}
        
        try {
            if constexpr (std::is_invocable_v<Func, const std::string&, const std::string&>) {
                auto val1 = std::any_cast<std::string>(arg1);
                auto val2 = std::any_cast<std::string>(arg2);
                if constexpr (std::is_void_v<std::invoke_result_t<Func, const std::string&, const std::string&>>) {
                    func(val1, val2);
                    return std::any{};
                } else {
                    return std::any(func(val1, val2));
                }
            }
        } catch (const std::bad_any_cast&) {}
        
        throw std::runtime_error("Cannot call function with provided argument types");
    }
};

/**
 * Simple registry for function groups
 */
class SimpleFunctionRegistry {
private:
    std::map<FunctionGroupType, std::unique_ptr<SimpleFunctionGroup>> groups_;
    static inline std::unique_ptr<SimpleFunctionRegistry> instance_;
    
public:
    static SimpleFunctionRegistry& instance() {
        if (!instance_) {
            instance_ = std::make_unique<SimpleFunctionRegistry>();
        }
        return *instance_;
    }
    
    void register_group(FunctionGroupType type, std::unique_ptr<SimpleFunctionGroup> group) {
        groups_[type] = std::move(group);
    }
    
    SimpleFunctionGroup* get_group(FunctionGroupType type) {
        auto it = groups_.find(type);
        return it != groups_.end() ? it->second.get() : nullptr;
    }
    
    std::vector<FunctionGroupType> group_types() const {
        std::vector<FunctionGroupType> types;
        for (const auto& [type, group] : groups_) {
            types.push_back(type);
        }
        return types;
    }
};
