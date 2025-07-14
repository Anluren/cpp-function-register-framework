#pragma once

#include "function_group.h"
#include <map>

/**
 * Singleton registry that manages all function groups
 * Each .cpp file can register its function groups here
 */
class FunctionRegistry {
private:
    std::map<FunctionGroupType, FunctionGroup*> groups;
    static FunctionRegistry* instance;
    
    // Private constructor for singleton
    FunctionRegistry() {}
    
public:
    // Get the singleton instance
    static FunctionRegistry& getInstance();
    
    // Clean up
    ~FunctionRegistry();
    
    // Register a function group
    void registerGroup(FunctionGroupType type, FunctionGroup* group);
    
    // Get a function group by type
    FunctionGroup* getGroup(FunctionGroupType type);
    
    // Check if a group exists
    bool hasGroup(FunctionGroupType type) const;
    
    // Get all registered group types
    std::vector<FunctionGroupType> getGroupTypes() const;
    
    // Remove a group
    void unregisterGroup(FunctionGroupType type);
    
    // Clear all groups
    void clear();
};

/**
 * Helper class for automatic registration
 * Use this in your .cpp files to automatically register function groups
 */
class AutoRegister {
public:
    AutoRegister(FunctionGroupType type, FunctionGroup* group) {
        FunctionRegistry::getInstance().registerGroup(type, group);
    }
};

// Macro to simplify registration in .cpp files
#define REGISTER_FUNCTION_GROUP(type, group) \
    static AutoRegister auto_register_##type(type, group)
