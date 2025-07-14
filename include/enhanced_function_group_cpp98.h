#pragma once

#include <string>
#include <vector>
#include <map>
#include <typeinfo>
#include <stdexcept>

/**
 * Enhanced function prototypes - C++98 Compatible Version
 * This version provides several improvements over the basic function group:
 * 
 * 1. Better type information and introspection
 * 2. Support for more parameter types
 * 3. Enhanced error handling
 * 4. Function metadata and documentation
 * 5. Parameter validation
 */

/**
 * Enum to identify different function groups
 */
enum FunctionGroupType {
    MATH_FUNCTIONS,
    STRING_FUNCTIONS,
    UTILITY_FUNCTIONS,
    CUSTOM_GROUP_1,
    CUSTOM_GROUP_2
};

/**
 * Enhanced function signature information
 */
struct FunctionInfo {
    std::string name;
    std::string description;
    std::string returnType;
    std::vector<std::string> parameterTypes;
    std::vector<std::string> parameterNames;
    int parameterCount;
    
    FunctionInfo() : parameterCount(0) {}
    
    FunctionInfo(const std::string& funcName, 
                 const std::string& desc,
                 const std::string& retType,
                 const std::vector<std::string>& paramTypes,
                 const std::vector<std::string>& paramNames)
        : name(funcName), description(desc), returnType(retType), 
          parameterTypes(paramTypes), parameterNames(paramNames),
          parameterCount(static_cast<int>(paramTypes.size())) {}
};

/**
 * Function call result with enhanced error information
 */
class FunctionCallResult {
private:
    void* data;
    std::string errorMessage;
    bool success;
    std::string returnTypeName;
    
public:
    FunctionCallResult() : data(0), success(false) {}
    
    FunctionCallResult(void* result, const std::string& typeName) 
        : data(result), success(true), returnTypeName(typeName) {}
    
    static FunctionCallResult createError(const std::string& error) {
        FunctionCallResult result;
        result.errorMessage = error;
        result.success = false;
        return result;
    }
    
    bool isSuccess() const { return success; }
    std::string getError() const { return errorMessage; }
    std::string getReturnType() const { return returnTypeName; }
    
    template<typename T>
    T getValue() const {
        if (!success) {
            throw std::runtime_error("Function call failed: " + errorMessage);
        }
        if (!data) {
            throw std::runtime_error("Null result data");
        }
        return *static_cast<T*>(data);
    }
    
    template<typename T>
    bool tryGetValue(T& output) const {
        if (!success || !data) {
            return false;
        }
        try {
            output = *static_cast<T*>(data);
            return true;
        } catch (...) {
            return false;
        }
    }
    
    void* getRawData() const { return data; }
};

/**
 * Enhanced function pointer types with more parameters
 */
typedef void* (*FuncPtr0)();
typedef void* (*FuncPtr1)(void*);
typedef void* (*FuncPtr2)(void*, void*);
typedef void* (*FuncPtr3)(void*, void*, void*);
typedef void* (*FuncPtr4)(void*, void*, void*, void*);
typedef void* (*FuncPtrVariadic)(void** args, int argCount);

/**
 * Function wrapper that stores both the function and its metadata
 */
class EnhancedFunctionWrapper {
private:
    FuncPtr0 func0;
    FuncPtr1 func1;
    FuncPtr2 func2;
    FuncPtr3 func3;
    FuncPtr4 func4;
    FuncPtrVariadic funcVariadic;
    FunctionInfo info;
    int actualParamCount;
    
public:
    EnhancedFunctionWrapper() 
        : func0(0), func1(0), func2(0), func3(0), func4(0), funcVariadic(0), actualParamCount(-1) {}
    
    // Register function with 0 parameters
    void setFunction(FuncPtr0 f, const FunctionInfo& functionInfo) {
        func0 = f;
        info = functionInfo;
        actualParamCount = 0;
    }
    
    // Register function with 1 parameter
    void setFunction(FuncPtr1 f, const FunctionInfo& functionInfo) {
        func1 = f;
        info = functionInfo;
        actualParamCount = 1;
    }
    
    // Register function with 2 parameters
    void setFunction(FuncPtr2 f, const FunctionInfo& functionInfo) {
        func2 = f;
        info = functionInfo;
        actualParamCount = 2;
    }
    
    // Register function with 3 parameters
    void setFunction(FuncPtr3 f, const FunctionInfo& functionInfo) {
        func3 = f;
        info = functionInfo;
        actualParamCount = 3;
    }
    
    // Register function with 4 parameters
    void setFunction(FuncPtr4 f, const FunctionInfo& functionInfo) {
        func4 = f;
        info = functionInfo;
        actualParamCount = 4;
    }
    
    // Register variadic function
    void setFunction(FuncPtrVariadic f, const FunctionInfo& functionInfo) {
        funcVariadic = f;
        info = functionInfo;
        actualParamCount = -1; // Variadic
    }
    
    FunctionCallResult call() const {
        if (actualParamCount != 0) {
            return FunctionCallResult::createError("Function requires " + 
                intToString(actualParamCount) + " parameters");
        }
        if (!func0) {
            return FunctionCallResult::createError("Function not registered");
        }
        
        try {
            void* result = func0();
            return FunctionCallResult(result, info.returnType);
        } catch (const std::exception& e) {
            return FunctionCallResult::createError(std::string("Exception: ") + e.what());
        } catch (...) {
            return FunctionCallResult::createError("Unknown exception occurred");
        }
    }
    
    FunctionCallResult call(void* arg1) const {
        if (actualParamCount != 1) {
            return FunctionCallResult::createError("Function requires " + 
                intToString(actualParamCount) + " parameters, got 1");
        }
        if (!func1) {
            return FunctionCallResult::createError("Function not registered");
        }
        
        try {
            void* result = func1(arg1);
            return FunctionCallResult(result, info.returnType);
        } catch (const std::exception& e) {
            return FunctionCallResult::createError(std::string("Exception: ") + e.what());
        } catch (...) {
            return FunctionCallResult::createError("Unknown exception occurred");
        }
    }
    
    FunctionCallResult call(void* arg1, void* arg2) const {
        if (actualParamCount != 2) {
            return FunctionCallResult::createError("Function requires " + 
                intToString(actualParamCount) + " parameters, got 2");
        }
        if (!func2) {
            return FunctionCallResult::createError("Function not registered");
        }
        
        try {
            void* result = func2(arg1, arg2);
            return FunctionCallResult(result, info.returnType);
        } catch (const std::exception& e) {
            return FunctionCallResult::createError(std::string("Exception: ") + e.what());
        } catch (...) {
            return FunctionCallResult::createError("Unknown exception occurred");
        }
    }
    
    FunctionCallResult call(void* arg1, void* arg2, void* arg3) const {
        if (actualParamCount != 3) {
            return FunctionCallResult::createError("Function requires " + 
                intToString(actualParamCount) + " parameters, got 3");
        }
        if (!func3) {
            return FunctionCallResult::createError("Function not registered");
        }
        
        try {
            void* result = func3(arg1, arg2, arg3);
            return FunctionCallResult(result, info.returnType);
        } catch (const std::exception& e) {
            return FunctionCallResult::createError(std::string("Exception: ") + e.what());
        } catch (...) {
            return FunctionCallResult::createError("Unknown exception occurred");
        }
    }
    
    FunctionCallResult call(void* arg1, void* arg2, void* arg3, void* arg4) const {
        if (actualParamCount != 4) {
            return FunctionCallResult::createError("Function requires " + 
                intToString(actualParamCount) + " parameters, got 4");
        }
        if (!func4) {
            return FunctionCallResult::createError("Function not registered");
        }
        
        try {
            void* result = func4(arg1, arg2, arg3, arg4);
            return FunctionCallResult(result, info.returnType);
        } catch (const std::exception& e) {
            return FunctionCallResult::createError(std::string("Exception: ") + e.what());
        } catch (...) {
            return FunctionCallResult::createError("Unknown exception occurred");
        }
    }
    
    FunctionCallResult callVariadic(void** args, int argCount) const {
        if (actualParamCount != -1) {
            return FunctionCallResult::createError("Function is not variadic");
        }
        if (!funcVariadic) {
            return FunctionCallResult::createError("Variadic function not registered");
        }
        
        try {
            void* result = funcVariadic(args, argCount);
            return FunctionCallResult(result, info.returnType);
        } catch (const std::exception& e) {
            return FunctionCallResult::createError(std::string("Exception: ") + e.what());
        } catch (...) {
            return FunctionCallResult::createError("Unknown exception occurred");
        }
    }
    
    const FunctionInfo& getFunctionInfo() const { return info; }
    int getParameterCount() const { return actualParamCount; }
    bool isVariadic() const { return actualParamCount == -1; }
    
private:
    std::string intToString(int value) const {
        if (value == -1) return "variable number of";
        
        std::string result;
        bool negative = value < 0;
        if (negative) value = -value;
        
        if (value == 0) return "0";
        
        while (value > 0) {
            result = char('0' + (value % 10)) + result;
            value /= 10;
        }
        
        if (negative) result = "-" + result;
        return result;
    }
};

/**
 * Enhanced function group with better introspection and error handling
 */
class EnhancedFunctionGroup {
private:
    std::string groupName;
    std::string groupDescription;
    std::string groupVersion;
    std::map<std::string, EnhancedFunctionWrapper> functions;
    
public:
    EnhancedFunctionGroup(const std::string& name, 
                         const std::string& description = "",
                         const std::string& version = "1.0.0")
        : groupName(name), groupDescription(description), groupVersion(version) {}
    
    virtual ~EnhancedFunctionGroup() {}
    
    // Group information
    std::string getName() const { return groupName; }
    std::string getDescription() const { return groupDescription; }
    std::string getVersion() const { return groupVersion; }
    
    // Enhanced function registration with metadata
    void registerFunction(const std::string& name, FuncPtr0 func, 
                         const std::string& description = "",
                         const std::string& returnType = "void*") {
        std::vector<std::string> paramTypes;
        std::vector<std::string> paramNames;
        FunctionInfo info(name, description, returnType, paramTypes, paramNames);
        functions[name].setFunction(func, info);
    }
    
    void registerFunction(const std::string& name, FuncPtr1 func,
                         const std::string& description = "",
                         const std::string& returnType = "void*",
                         const std::string& param1Type = "void*",
                         const std::string& param1Name = "arg1") {
        std::vector<std::string> paramTypes;
        std::vector<std::string> paramNames;
        paramTypes.push_back(param1Type);
        paramNames.push_back(param1Name);
        FunctionInfo info(name, description, returnType, paramTypes, paramNames);
        functions[name].setFunction(func, info);
    }
    
    void registerFunction(const std::string& name, FuncPtr2 func,
                         const std::string& description = "",
                         const std::string& returnType = "void*",
                         const std::string& param1Type = "void*",
                         const std::string& param1Name = "arg1",
                         const std::string& param2Type = "void*",
                         const std::string& param2Name = "arg2") {
        std::vector<std::string> paramTypes;
        std::vector<std::string> paramNames;
        paramTypes.push_back(param1Type);
        paramTypes.push_back(param2Type);
        paramNames.push_back(param1Name);
        paramNames.push_back(param2Name);
        FunctionInfo info(name, description, returnType, paramTypes, paramNames);
        functions[name].setFunction(func, info);
    }
    
    void registerFunction(const std::string& name, FuncPtr3 func,
                         const std::string& description = "",
                         const std::string& returnType = "void*",
                         const std::string& param1Type = "void*",
                         const std::string& param1Name = "arg1",
                         const std::string& param2Type = "void*",
                         const std::string& param2Name = "arg2",
                         const std::string& param3Type = "void*",
                         const std::string& param3Name = "arg3") {
        std::vector<std::string> paramTypes;
        std::vector<std::string> paramNames;
        paramTypes.push_back(param1Type);
        paramTypes.push_back(param2Type);
        paramTypes.push_back(param3Type);
        paramNames.push_back(param1Name);
        paramNames.push_back(param2Name);
        paramNames.push_back(param3Name);
        FunctionInfo info(name, description, returnType, paramTypes, paramNames);
        functions[name].setFunction(func, info);
    }
    
    void registerFunction(const std::string& name, FuncPtr4 func,
                         const std::string& description = "",
                         const std::string& returnType = "void*",
                         const std::string& param1Type = "void*",
                         const std::string& param1Name = "arg1",
                         const std::string& param2Type = "void*",
                         const std::string& param2Name = "arg2",
                         const std::string& param3Type = "void*",
                         const std::string& param3Name = "arg3",
                         const std::string& param4Type = "void*",
                         const std::string& param4Name = "arg4") {
        std::vector<std::string> paramTypes;
        std::vector<std::string> paramNames;
        paramTypes.push_back(param1Type);
        paramTypes.push_back(param2Type);
        paramTypes.push_back(param3Type);
        paramTypes.push_back(param4Type);
        paramNames.push_back(param1Name);
        paramNames.push_back(param2Name);
        paramNames.push_back(param3Name);
        paramNames.push_back(param4Name);
        FunctionInfo info(name, description, returnType, paramTypes, paramNames);
        functions[name].setFunction(func, info);
    }
    
    // Function introspection
    std::vector<std::string> getFunctionNames() const {
        std::vector<std::string> names;
        for (std::map<std::string, EnhancedFunctionWrapper>::const_iterator it = functions.begin();
             it != functions.end(); ++it) {
            names.push_back(it->first);
        }
        return names;
    }
    
    std::vector<FunctionInfo> getFunctionInfos() const {
        std::vector<FunctionInfo> infos;
        for (std::map<std::string, EnhancedFunctionWrapper>::const_iterator it = functions.begin();
             it != functions.end(); ++it) {
            infos.push_back(it->second.getFunctionInfo());
        }
        return infos;
    }
    
    bool hasFunction(const std::string& functionName) const {
        return functions.find(functionName) != functions.end();
    }
    
    FunctionInfo getFunctionInfo(const std::string& functionName) const {
        std::map<std::string, EnhancedFunctionWrapper>::const_iterator it = functions.find(functionName);
        if (it != functions.end()) {
            return it->second.getFunctionInfo();
        }
        return FunctionInfo(); // Return empty info if not found
    }
    
    // Enhanced function calling with better error handling
    FunctionCallResult callFunction(const std::string& functionName) const {
        std::map<std::string, EnhancedFunctionWrapper>::const_iterator it = functions.find(functionName);
        if (it == functions.end()) {
            return FunctionCallResult::createError("Function not found: " + functionName);
        }
        return it->second.call();
    }
    
    FunctionCallResult callFunction(const std::string& functionName, void* arg1) const {
        std::map<std::string, EnhancedFunctionWrapper>::const_iterator it = functions.find(functionName);
        if (it == functions.end()) {
            return FunctionCallResult::createError("Function not found: " + functionName);
        }
        return it->second.call(arg1);
    }
    
    FunctionCallResult callFunction(const std::string& functionName, void* arg1, void* arg2) const {
        std::map<std::string, EnhancedFunctionWrapper>::const_iterator it = functions.find(functionName);
        if (it == functions.end()) {
            return FunctionCallResult::createError("Function not found: " + functionName);
        }
        return it->second.call(arg1, arg2);
    }
    
    FunctionCallResult callFunction(const std::string& functionName, void* arg1, void* arg2, void* arg3) const {
        std::map<std::string, EnhancedFunctionWrapper>::const_iterator it = functions.find(functionName);
        if (it == functions.end()) {
            return FunctionCallResult::createError("Function not found: " + functionName);
        }
        return it->second.call(arg1, arg2, arg3);
    }
    
    FunctionCallResult callFunction(const std::string& functionName, void* arg1, void* arg2, void* arg3, void* arg4) const {
        std::map<std::string, EnhancedFunctionWrapper>::const_iterator it = functions.find(functionName);
        if (it == functions.end()) {
            return FunctionCallResult::createError("Function not found: " + functionName);
        }
        return it->second.call(arg1, arg2, arg3, arg4);
    }
    
    // Utility methods
    void printFunctionList() const {
        std::vector<std::string> names = getFunctionNames();
        for (size_t i = 0; i < names.size(); ++i) {
            FunctionInfo info = getFunctionInfo(names[i]);
            // Note: In a real implementation, you'd use proper logging
            // std::cout << names[i] << ": " << info.description << std::endl;
        }
    }
};
