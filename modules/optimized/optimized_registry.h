#pragma once

#include "optimized_function_group.h"
#include <memory>
#include <array>

namespace Optimized {

/**
 * High-performance singleton registry
 * Uses array-based storage for known categories and minimal allocation
 */
class OptimizedRegistry {
private:
    static constexpr size_t MAX_CATEGORIES = 8;
    std::array<std::unique_ptr<OptimizedFunctionGroup>, MAX_CATEGORIES> groups;
    static std::unique_ptr<OptimizedRegistry> instance;
    
    OptimizedRegistry() = default;

public:
    // Fast singleton access
    static OptimizedRegistry& getInstance() {
        if (!instance) {
            instance.reset(new OptimizedRegistry());
        }
        return *instance;
    }
    
    // Register a function group
    void registerGroup(FunctionCategory category, std::string_view name) {
        auto index = static_cast<size_t>(category);
        if (index < MAX_CATEGORIES) {
            groups[index].reset(new OptimizedFunctionGroup(name, category));
        }
    }
    
    // Get function group (very fast array access)
    OptimizedFunctionGroup* getGroup(FunctionCategory category) {
        auto index = static_cast<size_t>(category);
        return (index < MAX_CATEGORIES) ? groups[index].get() : nullptr;
    }
    
    // Register function directly
    template<typename R, typename... Args>
    bool registerFunction(FunctionCategory category, std::string_view name, R(*func)(Args...)) {
        auto* group = getGroup(category);
        if (group) {
            group->registerFunction(name, func);
            return true;
        }
        return false;
    }
    
    // Call function directly with minimal overhead
    template<typename R, typename... Args>
    R callFunction(FunctionCategory category, std::string_view name, Args... args) {
        auto* group = getGroup(category);
        if (group) {
            return group->template callFunction<R>(name, args...);
        }
        throw std::runtime_error("Group not found");
    }
    
    // Get function pointer for maximum performance
    template<typename R, typename... Args>
    R(*getFunction(FunctionCategory category, std::string_view name))(Args...) {
        auto* group = getGroup(category);
        return group ? group->template getFunction<R, Args...>(name) : nullptr;
    }
    
    // Check if group exists
    bool hasGroup(FunctionCategory category) const {
        auto index = static_cast<size_t>(category);
        return (index < MAX_CATEGORIES) && groups[index] != nullptr;
    }
    
    // Get all active categories
    std::vector<FunctionCategory> getCategories() const {
        std::vector<FunctionCategory> categories;
        for (size_t i = 0; i < MAX_CATEGORIES; ++i) {
            if (groups[i]) {
                categories.push_back(static_cast<FunctionCategory>(i));
            }
        }
        return categories;
    }
    
    // Clear all groups
    void clear() {
        for (auto& group : groups) {
            group.reset();
        }
    }
};

/**
 * High-performance registration macros
 */
#define OPTIMIZED_REGISTER_GROUP(category, name) \
    Optimized::OptimizedRegistry::getInstance().registerGroup(category, name)

#define OPTIMIZED_REGISTER_FUNCTION(category, name, func) \
    Optimized::OptimizedRegistry::getInstance().registerFunction(category, name, func)

#define OPTIMIZED_GET_FUNCTION(category, name, funcType) \
    Optimized::OptimizedRegistry::getInstance().getFunction<funcType>(category, name)

#define OPTIMIZED_CALL_FUNCTION(category, name, retType, ...) \
    Optimized::OptimizedRegistry::getInstance().callFunction<retType>(category, name, __VA_ARGS__)

} // namespace Optimized

// Global initialization function
namespace Optimized {
    void initializeOptimizedModule();
}
