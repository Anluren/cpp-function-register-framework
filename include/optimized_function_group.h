#pragma once

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <functional>
#include <any>
#include <optional>
#include <memory>
#include <type_traits>
#include "simple_modern_function_group.h" // For FunctionGroupType

/**
 * Function ID for fast lookups (can be generated at compile time)
 */
using FunctionId = std::size_t;

/**
 * Compile-time string hash for function IDs
 */
constexpr FunctionId hash_string(const char* str) {
    std::size_t hash = 5381;
    while (*str) {
        hash = ((hash << 5) + hash) + static_cast<unsigned char>(*str++);
    }
    return hash;
}

/**
 * Macro to create compile-time function IDs
 */
#define FUNC_ID(name) hash_string(name)

/**
 * Performance-optimized function group with multiple lookup strategies
 */
class OptimizedFunctionGroup {
private:
    std::string name_;
    // Primary storage: unordered_map for O(1) average lookup
    std::unordered_map<FunctionId, std::function<std::any(const std::vector<std::any>&)>> functions_by_id_;
    // Secondary storage: map for string-based lookup (backward compatibility)
    std::map<std::string, FunctionId> name_to_id_;
    
public:
    explicit OptimizedFunctionGroup(std::string name) : name_(std::move(name)) {}
    
    // Get group information
    const std::string& name() const { return name_; }
    
    std::vector<std::string> function_names() const {
        std::vector<std::string> names;
        for (const auto& [name, id] : name_to_id_) {
            names.push_back(name);
        }
        return names;
    }
    
    bool has_function(FunctionId id) const {
        return functions_by_id_.find(id) != functions_by_id_.end();
    }
    
    bool has_function(const std::string& name) const {
        return name_to_id_.find(name) != name_to_id_.end();
    }
    
    // Fast registration with compile-time ID
    template<typename Func>
    void add(FunctionId id, const std::string& name, Func&& func) {
        functions_by_id_[id] = create_wrapper(std::forward<Func>(func));
        name_to_id_[name] = id;
    }
    
    // Backward compatible registration
    template<typename Func>
    void add(const std::string& name, Func&& func) {
        FunctionId id = std::hash<std::string>{}(name);
        add(id, name, std::forward<Func>(func));
    }
    
    // Fast call with compile-time ID - O(1) average
    template<typename... Args>
    std::any call(FunctionId id, Args&&... args) const {
        auto it = functions_by_id_.find(id);
        if (it == functions_by_id_.end()) {
            throw std::runtime_error("Function not found (ID: " + std::to_string(id) + ")");
        }
        
        std::vector<std::any> arg_vector;
        if constexpr (sizeof...(args) > 0) {
            (arg_vector.emplace_back(std::forward<Args>(args)), ...);
        }
        
        return it->second(arg_vector);
    }
    
    // Backward compatible call with string lookup - O(log n)
    template<typename... Args>
    std::any call(const std::string& name, Args&&... args) const {
        auto name_it = name_to_id_.find(name);
        if (name_it == name_to_id_.end()) {
            throw std::runtime_error("Function not found: " + name);
        }
        return call(name_it->second, std::forward<Args>(args)...);
    }
    
    // Fast type-safe call with compile-time ID
    template<typename ReturnType, typename... Args>
    ReturnType call_as(FunctionId id, Args&&... args) const {
        auto result = call(id, std::forward<Args>(args)...);
        if constexpr (std::is_void_v<ReturnType>) {
            // For void functions, we don't return anything
        } else {
            return std::any_cast<ReturnType>(result);
        }
    }
    
    // Backward compatible type-safe call
    template<typename ReturnType, typename... Args>
    ReturnType call_as(const std::string& name, Args&&... args) const {
        auto result = call(name, std::forward<Args>(args)...);
        if constexpr (std::is_void_v<ReturnType>) {
            // For void functions, we don't return anything
        } else {
            return std::any_cast<ReturnType>(result);
        }
    }
    
    // Fast safe call with compile-time ID
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> try_call(FunctionId id, Args&&... args) const {
        try {
            if constexpr (std::is_void_v<ReturnType>) {
                call(id, std::forward<Args>(args)...);
                return std::nullopt; // void functions return nullopt to indicate success
            } else {
                auto result = call(id, std::forward<Args>(args)...);
                return std::any_cast<ReturnType>(result);
            }
        } catch (...) {
            return std::nullopt;
        }
    }
    
    // Backward compatible safe call
    template<typename ReturnType, typename... Args>
    std::optional<ReturnType> try_call(const std::string& name, Args&&... args) const {
        try {
            if constexpr (std::is_void_v<ReturnType>) {
                call(name, std::forward<Args>(args)...);
                return std::nullopt;
            } else {
                auto result = call(name, std::forward<Args>(args)...);
                return std::any_cast<ReturnType>(result);
            }
        } catch (...) {
            return std::nullopt;
        }
    }

private:
    // Same create_wrapper implementation as before
    template<typename Func>
    auto create_wrapper(Func&& func) -> std::function<std::any(const std::vector<std::any>&)> {
        return [this, f = std::forward<Func>(func)](const std::vector<std::any>& args) -> std::any {
            if (args.empty()) {
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
                return this->call_with_one_arg(f, args[0]);
            } else if (args.size() == 2) {
                return this->call_with_two_args(f, args[0], args[1]);
            } else {
                throw std::runtime_error("Functions with more than 2 arguments not yet supported");
            }
        };
    }
    
    template<typename Func>
    std::any call_with_one_arg(Func&& func, const std::any& arg) const {
        // Try different single argument types (same implementation as before)
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
        // Try different two argument type combinations (same implementation as before)
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
 * Performance-optimized registry
 */
class OptimizedFunctionRegistry {
private:
    std::map<FunctionGroupType, std::unique_ptr<OptimizedFunctionGroup>> groups_;
    static inline std::unique_ptr<OptimizedFunctionRegistry> instance_;
    
public:
    static OptimizedFunctionRegistry& instance() {
        if (!instance_) {
            instance_ = std::make_unique<OptimizedFunctionRegistry>();
        }
        return *instance_;
    }
    
    void register_group(FunctionGroupType type, std::unique_ptr<OptimizedFunctionGroup> group) {
        groups_[type] = std::move(group);
    }
    
    OptimizedFunctionGroup* get_group(FunctionGroupType type) {
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
    
    // Direct fast call without group lookup
    template<typename ReturnType, typename... Args>
    ReturnType fast_call(FunctionGroupType group_type, FunctionId func_id, Args&&... args) {
        auto* group = get_group(group_type);
        if (!group) {
            throw std::runtime_error("Function group not found");
        }
        return group->template call_as<ReturnType>(func_id, std::forward<Args>(args)...);
    }
};
