#include "function_group.h"
#include <algorithm>

ConcreteFunctionGroup::ConcreteFunctionGroup(const std::string& name) : groupName(name) {
}

std::string ConcreteFunctionGroup::getName() const {
    return groupName;
}

std::vector<std::string> ConcreteFunctionGroup::getFunctionNames() const {
    std::vector<std::string> names;
    
    // Add all function names from different maps
    for (std::map<std::string, FuncPtr0>::const_iterator it = functions0.begin(); 
         it != functions0.end(); ++it) {
        names.push_back(it->first);
    }
    
    for (std::map<std::string, FuncPtr1>::const_iterator it = functions1.begin(); 
         it != functions1.end(); ++it) {
        names.push_back(it->first);
    }
    
    for (std::map<std::string, FuncPtr2>::const_iterator it = functions2.begin(); 
         it != functions2.end(); ++it) {
        names.push_back(it->first);
    }
    
    return names;
}

bool ConcreteFunctionGroup::hasFunction(const std::string& functionName) const {
    return functions0.find(functionName) != functions0.end() ||
           functions1.find(functionName) != functions1.end() ||
           functions2.find(functionName) != functions2.end();
}

void* ConcreteFunctionGroup::callFunction(const std::string& functionName, void* args) const {
    // Try to find function with no parameters first
    std::map<std::string, FuncPtr0>::const_iterator it0 = functions0.find(functionName);
    if (it0 != functions0.end()) {
        return it0->second();
    }
    
    // Try to find function with one parameter
    std::map<std::string, FuncPtr1>::const_iterator it1 = functions1.find(functionName);
    if (it1 != functions1.end()) {
        return it1->second(args);
    }
    
    // Try to find function with two parameters (args should point to array of void*)
    std::map<std::string, FuncPtr2>::const_iterator it2 = functions2.find(functionName);
    if (it2 != functions2.end()) {
        void** argArray = static_cast<void**>(args);
        return it2->second(argArray[0], argArray[1]);
    }
    
    return 0; // Function not found
}

void ConcreteFunctionGroup::registerFunction0(const std::string& name, FuncPtr0 func) {
    functions0[name] = func;
}

void ConcreteFunctionGroup::registerFunction1(const std::string& name, FuncPtr1 func) {
    functions1[name] = func;
}

void ConcreteFunctionGroup::registerFunction2(const std::string& name, FuncPtr2 func) {
    functions2[name] = func;
}

void* ConcreteFunctionGroup::callFunction0(const std::string& functionName) const {
    std::map<std::string, FuncPtr0>::const_iterator it = functions0.find(functionName);
    if (it != functions0.end()) {
        return it->second();
    }
    return 0;
}

void* ConcreteFunctionGroup::callFunction1(const std::string& functionName, void* arg) const {
    std::map<std::string, FuncPtr1>::const_iterator it = functions1.find(functionName);
    if (it != functions1.end()) {
        return it->second(arg);
    }
    return 0;
}

void* ConcreteFunctionGroup::callFunction2(const std::string& functionName, void* arg1, void* arg2) const {
    std::map<std::string, FuncPtr2>::const_iterator it = functions2.find(functionName);
    if (it != functions2.end()) {
        return it->second(arg1, arg2);
    }
    return 0;
}
