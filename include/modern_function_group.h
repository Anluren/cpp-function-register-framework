#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <any>
#include <optional>
#include <memory>
#include <type_traits>
#include <tuple>

/**
 * Enum to identify different function groups (using C++17 style)
 */
enum class FunctionGroupType {
    MATH_FUNCTIONS,
    STRING_FUNCTIONS,
    UTILITY_FUNCTIONS,
    CUSTOM_GROUP_1,
    CUSTOM_GROUP_2
};

/**
 * Modern function group using C++17 features for simple and unified function registration
 * Supports function pointers, lambdas, and std::function objects with automatic type deduction
 */
class ModernFunctionGroup {
private:
    std::string name_;
    std::map<std::string, std::function<std::any(const std::vector<std::any>&)>> functions_;
    
    // Helper to extract function signature
    template<typename T>
    struct function_traits;
    
    // Specialization for function pointers
    template<typename R, typename... Args>
    struct function_traits<R(*)(Args...)> {
        using return_type = R;
        using args_tuple = std::tuple<Args...>;
        static constexpr size_t arity = sizeof...(Args);
    };
    
    // Specialization for lambdas and function objects
    template<typename T>
    struct function_traits : public function_traits<decltype(&T::operator())> {};
    
    // Specialization for member function pointers (lambdas)
    template<typename ClassType, typename R, typename... Args>
    struct function_traits<R(ClassType::*)(Args...) const> {
        using return_type = R;
        using args_tuple = std::tuple<Args...>;
        static constexpr size_t arity = sizeof...(Args);
    };
    
    // Helper to call function with unpacked arguments
    template<typename Func, size_t... I>
    auto call_with_indices(Func&& func, const std::vector<std::any>& args, std::index_sequence<I...>) {
        using traits = function_traits<std::decay_t<Func>>;
        using args_tuple = typename traits::args_tuple;
        
        if (args.size() != sizeof...(I)) {
            throw std::runtime_error("Function expects " + std::to_string(sizeof...(I)) + " arguments");
        }
        
        if constexpr (std::is_void_v<typename traits::return_type>) {
            func(std::any_cast<std::tuple_element_t<I, args_tuple>>(args[I])...);
            return std::any{};
        } else {
            return std::any(func(std::any_cast<std::tuple_element_t<I, args_tuple>>(args[I])...));
        }
    }
    
public:
    explicit ModernFunctionGroup(std::string name) : name_(std::move(name)) {}
    
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
    
    // Unified registration for any callable (function pointers, lambdas, std::function)
    template<typename Func>
    void add(const std::string& name, Func&& func) {
        using traits = function_traits<std::decay_t<Func>>;
        constexpr size_t arity = traits::arity;
        
        functions_[name] = [this, f = std::forward<Func>(func)](const std::vector<std::any>& args) -> std::any {
            return this->call_with_indices(f, args, std::make_index_sequence<arity>{});
        };
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
            return;
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
                return std::nullopt; // void functions return nullopt
            } else {
                auto result = call(name, std::forward<Args>(args)...);
                return std::any_cast<ReturnType>(result);
            }
        } catch (...) {
            return std::nullopt;
        }
    }
private:
    // Helper method to make call_with_indices work with the current class
    template<typename Func, size_t... I>
    std::any call_with_indices(Func&& func, const std::vector<std::any>& args, std::index_sequence<I...>) const {
        using traits = function_traits<std::decay_t<Func>>;
        using args_tuple = typename traits::args_tuple;
        
        if (args.size() != sizeof...(I)) {
            throw std::runtime_error("Function expects " + std::to_string(sizeof...(I)) + " arguments");
        }
        
        if constexpr (std::is_void_v<typename traits::return_type>) {
            func(std::any_cast<std::tuple_element_t<I, args_tuple>>(args[I])...);
            return std::any{};
        } else {
            return std::any(func(std::any_cast<std::tuple_element_t<I, args_tuple>>(args[I])...));
        }
    }
};

/**
 * Simple registry for function groups
 */
class FunctionRegistry {
private:
    std::map<FunctionGroupType, std::unique_ptr<ModernFunctionGroup>> groups_;
    static inline std::unique_ptr<FunctionRegistry> instance_;
    
public:
    static FunctionRegistry& instance() {
        if (!instance_) {
            instance_ = std::make_unique<FunctionRegistry>();
        }
        return *instance_;
    }
    
    void register_group(FunctionGroupType type, std::unique_ptr<ModernFunctionGroup> group) {
        groups_[type] = std::move(group);
    }
    
    ModernFunctionGroup* get_group(FunctionGroupType type) {
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
