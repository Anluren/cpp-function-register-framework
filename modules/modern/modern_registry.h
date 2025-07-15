#pragma once

#include "modern_function_group.h"
#include <memory>
#include <unordered_map>
#include <mutex>

namespace Modern {

/**
 * Modern singleton registry using smart pointers and unordered_map
 */
class ModernRegistry {
private:
    std::unordered_map<FunctionCategory, std::unique_ptr<ModernFunctionGroup> > groups;
    static std::unique_ptr<ModernRegistry> instance;
    static std::once_flag initFlag;
    
    ModernRegistry() = default;

public:
    // Get singleton instance (thread-safe)
    static ModernRegistry& getInstance() {
        std::call_once(initFlag, []() {
            instance = std::unique_ptr<ModernRegistry>(new ModernRegistry());
        });
        return *instance;
    }
    
    // No copy/move
    ModernRegistry(const ModernRegistry&) = delete;
    ModernRegistry& operator=(const ModernRegistry&) = delete;
    ModernRegistry(ModernRegistry&&) = delete;
    ModernRegistry& operator=(ModernRegistry&&) = delete;
    
    // Register a function group
    void registerGroup(FunctionCategory category, const std::string& name) {
        groups[category] = std::unique_ptr<ModernFunctionGroup>(new ModernFunctionGroup(name, category));
    }
    
    // Get a function group
    ModernFunctionGroup* getGroup(FunctionCategory category) {
        auto it = groups.find(category);
        return (it != groups.end()) ? it->second.get() : nullptr;
    }
    
    // Register a function in a group
    template<typename Signature>
    bool registerFunction(FunctionCategory category, const std::string& name, 
                         std::function<Signature> func) {
        auto* group = getGroup(category);
        if (group) {
            group->registerFunction<Signature>(name, std::move(func));
            return true;
        }
        return false;
    }
    
    // Register a lambda in a group - explicit signature version
    template<typename Signature, typename F>
    bool registerLambda(FunctionCategory category, const std::string& name, F&& func) {
        auto* group = getGroup(category);
        if (group) {
            group->template registerLambda<Signature>(name, std::forward<F>(func));
            return true;
        }
        return false;
    }
    
    // Get a typed function
    template<typename Signature>
    std::shared_ptr<TypedFunction<Signature> > getFunction(FunctionCategory category, 
                                                         const std::string& name) {
        auto* group = getGroup(category);
        return group ? group->getFunction<Signature>(name) : nullptr;
    }
    
    // Check if group exists
    bool hasGroup(FunctionCategory category) const {
        return groups.find(category) != groups.end();
    }
    
    // Get all registered categories
    std::vector<FunctionCategory> getCategories() const {
        std::vector<FunctionCategory> categories;
        for (const auto& pair : groups) {
            categories.push_back(pair.first);
        }
        return categories;
    }
    
    // Clear all groups
    void clear() {
        groups.clear();
    }
    
    // Initialize all modules (call this to ensure static registration)
    void initializeModules();
};

/**
 * Helper macros for easy registration
 */
#define MODERN_REGISTER_GROUP(category, name) \
    Modern::ModernRegistry::getInstance().registerGroup(category, name)

#define MODERN_REGISTER_FUNC(category, name, func) \
    Modern::ModernRegistry::getInstance().registerFunction(category, name, func)

#define MODERN_REGISTER_LAMBDA_FUNC(category, name, signature, lambda) \
    Modern::ModernRegistry::getInstance().registerLambda<signature>(category, name, lambda)

#define MODERN_GET_FUNCTION(category, name, signature) \
    Modern::ModernRegistry::getInstance().getFunction<signature>(category, name)

} // namespace Modern
