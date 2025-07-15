#pragma once

#include "legacy_function_group.h"
#include <memory>

namespace Legacy {

/**
 * Simple singleton registry for legacy function groups
 * Provides basic registration and lookup functionality
 * Now using C++17 features for better code quality
 */
class LegacyRegistry {
private:
    std::map<FunctionType, std::unique_ptr<LegacyFunctionGroup>> groups;
    static std::unique_ptr<LegacyRegistry> instance;
    
    LegacyRegistry() = default;

public:
    // Get singleton instance
    static LegacyRegistry& getInstance() {
        if (!instance) {
            instance.reset(new LegacyRegistry());
        }
        return *instance;
    }
    
    // Clean up is automatic with unique_ptr
    ~LegacyRegistry() = default;
    
    // Register a function group
    void registerGroup(FunctionType type, const std::string& name) {
        groups[type] = std::unique_ptr<LegacyFunctionGroup>(new LegacyFunctionGroup(name, type));
    }
    
    // Get a function group
    LegacyFunctionGroup* getGroup(FunctionType type) {
        auto it = groups.find(type);
        return (it != groups.end()) ? it->second.get() : nullptr;
    }
    
    // Register a function in a group
    bool registerFunction(FunctionType groupType, const std::string& funcName, void* funcPtr) {
        auto* group = getGroup(groupType);
        if (group) {
            group->registerFunction(funcName, funcPtr);
            return true;
        }
        return false;
    }
    
    // Get a function from a group
    void* getFunction(FunctionType groupType, const std::string& funcName) {
        auto* group = getGroup(groupType);
        return group ? group->getFunction(funcName) : nullptr;
    }
    
    // Check if group exists
    bool hasGroup(FunctionType type) const {
        return groups.find(type) != groups.end();
    }
    
    // Get all registered group types
    std::vector<FunctionType> getGroupTypes() const {
        std::vector<FunctionType> types;
        for (const auto& pair : groups) {
            types.push_back(pair.first);
        }
        return types;
    }
    
    // Clear all groups
    void clear() {
        groups.clear();
    }
};

/**
 * Helper macros for easy function registration
 */
#define LEGACY_REGISTER_GROUP(type, name) \
    Legacy::LegacyRegistry::getInstance().registerGroup(type, name)

#define LEGACY_REGISTER_FUNCTION(groupType, name, func) \
    Legacy::LegacyRegistry::getInstance().registerFunction(groupType, name, (void*)func)

#define LEGACY_GET_FUNCTION(groupType, name, funcType) \
    (funcType)Legacy::LegacyRegistry::getInstance().getFunction(groupType, name)

} // namespace Legacy
