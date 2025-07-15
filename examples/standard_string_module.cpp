#include "../include/standard_function_registry.h"
#include <iostream>
#include <algorithm>

/**
 * String Module - implements only string-related standard functions
 */

namespace string_module {

// Implementation of standard string functions
std::string to_upper_impl(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
    }
    return result;
}

std::string to_lower_impl(const std::string& str) {
    std::string result = str;
    for (char& c : result) {
        if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 'a';
        }
    }
    return result;
}

std::string concat_impl(const std::string& a, const std::string& b) {
    return a + " " + b;
}

int get_length_impl(const std::string& str) {
    return static_cast<int>(str.length());
}

// Module registration function
void register_string_module() {
    ModuleRegistrar registrar("StringModule");
    
    std::cout << "Registering String Module functions..." << std::endl;
    
    // Register only the string functions this module supports
    registrar.register_function(StandardFunction::TO_UPPER, to_upper_impl);
    registrar.register_function(StandardFunction::TO_LOWER, to_lower_impl);
    registrar.register_function(StandardFunction::CONCAT, concat_impl);
    registrar.register_function(StandardFunction::GET_LENGTH, get_length_impl);
    
    // Note: This module does NOT implement math, file, or network functions
    
    std::cout << "String Module registered 4 functions." << std::endl;
}

} // namespace string_module

// Automatic registration
namespace {
    struct StringModuleAutoRegistration {
        StringModuleAutoRegistration() {
            string_module::register_string_module();
        }
    };
    static StringModuleAutoRegistration auto_register;
}
