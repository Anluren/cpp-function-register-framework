#include "function_registry.h"
#include <string>
#include <cstring>
#include <algorithm>

// Example string functions
void* str_length_func(void* str_ptr) {
    const char* str = static_cast<const char*>(str_ptr);
    static int result = strlen(str);
    return &result;
}

void* str_upper_func(void* str_ptr) {
    const char* input = static_cast<const char*>(str_ptr);
    static std::string result = input;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return const_cast<char*>(result.c_str());
}

void* str_concat_func(void* str1_ptr, void* str2_ptr) {
    const char* str1 = static_cast<const char*>(str1_ptr);
    const char* str2 = static_cast<const char*>(str2_ptr);
    static std::string result = std::string(str1) + std::string(str2);
    return const_cast<char*>(result.c_str());
}

void* str_reverse_func(void* str_ptr) {
    const char* input = static_cast<const char*>(str_ptr);
    static std::string result = input;
    std::reverse(result.begin(), result.end());
    return const_cast<char*>(result.c_str());
}

// Create and register the string function group
void initStringFunctions() {
    ConcreteFunctionGroup* stringGroup = new ConcreteFunctionGroup("String Functions");
    
    // Register functions
    stringGroup->registerFunction1("length", str_length_func);
    stringGroup->registerFunction1("upper", str_upper_func);
    stringGroup->registerFunction1("reverse", str_reverse_func);
    stringGroup->registerFunction2("concat", str_concat_func);
    
    // Register the group
    FunctionRegistry::getInstance().registerGroup(STRING_FUNCTIONS, stringGroup);
}

// Use static initialization to register automatically
static class StringFunctionInitializer {
public:
    StringFunctionInitializer() {
        initStringFunctions();
    }
} stringInit;
