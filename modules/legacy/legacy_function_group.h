#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>

namespace Legacy {

/**
 * Simple function group types for legacy compatibility
 */
enum FunctionType {
    MATH = 0,
    STRING = 1,
    UTILITY = 2
};

/**
 * Basic function group for legacy applications
 * Uses simple void* casting for maximum compatibility
 */
class LegacyFunctionGroup {
private:
    std::string groupName;
    std::map<std::string, void*> functions;
    FunctionType type;

public:
    LegacyFunctionGroup(const std::string& name, FunctionType t) 
        : groupName(name), type(t) {}
    
    virtual ~LegacyFunctionGroup() {}
    
    // Register a function with void* pointer
    void registerFunction(const std::string& name, void* funcPtr) {
        functions[name] = funcPtr;
    }
    
    // Get function pointer by name
    void* getFunction(const std::string& name) const {
        auto it = functions.find(name);
        return (it != functions.end()) ? it->second : nullptr;
    }
    
    // Check if function exists
    bool hasFunction(const std::string& name) const {
        return functions.find(name) != functions.end();
    }
    
    // Get all function names
    std::vector<std::string> getFunctionNames() const {
        std::vector<std::string> names;
        for (const auto& pair : functions) {
            names.push_back(pair.first);
        }
        return names;
    }
    
    // Get group info
    std::string getName() const { return groupName; }
    FunctionType getType() const { return type; }
    size_t getFunctionCount() const { return functions.size(); }
};

/**
 * Legacy function type definitions for common signatures
 */
typedef int (*IntFunction)();
typedef int (*IntIntFunction)(int);
typedef int (*IntIntIntFunction)(int, int);
typedef double (*DoubleFunction)();
typedef double (*DoubleDoubleFunction)(double);
typedef double (*DoubleDoubleDoubleFunction)(double, double);
typedef const char* (*StringFunction)();
typedef const char* (*StringStringFunction)(const char*);
typedef const char* (*StringStringStringFunction)(const char*, const char*);

} // namespace Legacy
