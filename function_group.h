#pragma once

#include <string>
#include <vector>
#include <map>

/**
 * Enum to identify different function groups
 * Add your custom function group types here
 */
enum FunctionGroupType {
    MATH_FUNCTIONS,
    STRING_FUNCTIONS,
    UTILITY_FUNCTIONS,
    CUSTOM_GROUP_1,
    CUSTOM_GROUP_2
};

/**
 * Base class for function groups
 * Each group can contain multiple callable functions
 */
class FunctionGroup {
public:
    virtual ~FunctionGroup() {}
    
    // Get the name of this function group
    virtual std::string getName() const = 0;
    
    // Get list of available function names in this group
    virtual std::vector<std::string> getFunctionNames() const = 0;
    
    // Check if a function exists in this group
    virtual bool hasFunction(const std::string& functionName) const = 0;
    
    // Generic function call interface
    virtual void* callFunction(const std::string& functionName, void* args = 0) const = 0;
};

/**
 * Function pointer types for different signatures
 */
typedef void* (*FuncPtr0)();
typedef void* (*FuncPtr1)(void*);
typedef void* (*FuncPtr2)(void*, void*);

/**
 * Concrete implementation for function groups
 */
class ConcreteFunctionGroup : public FunctionGroup {
private:
    std::string groupName;
    std::map<std::string, FuncPtr0> functions0;      // No parameters
    std::map<std::string, FuncPtr1> functions1;      // One parameter
    std::map<std::string, FuncPtr2> functions2;      // Two parameters
    
public:
    explicit ConcreteFunctionGroup(const std::string& name);
    
    virtual ~ConcreteFunctionGroup() {}
    
    std::string getName() const;
    std::vector<std::string> getFunctionNames() const;
    bool hasFunction(const std::string& functionName) const;
    void* callFunction(const std::string& functionName, void* args = 0) const;
    
    // Register functions with different signatures
    void registerFunction0(const std::string& name, FuncPtr0 func);
    void registerFunction1(const std::string& name, FuncPtr1 func);
    void registerFunction2(const std::string& name, FuncPtr2 func);
    
    // Call functions with specific signatures
    void* callFunction0(const std::string& functionName) const;
    void* callFunction1(const std::string& functionName, void* arg) const;
    void* callFunction2(const std::string& functionName, void* arg1, void* arg2) const;
};
