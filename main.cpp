#include "function_registry.h"
#include <iostream>
#include <vector>

int main() {
    std::cout << "=== Function Registration Framework Demo ===" << std::endl;
    
    FunctionRegistry& registry = FunctionRegistry::getInstance();
    
    // Show all registered groups
    std::vector<FunctionGroupType> groupTypes = registry.getGroupTypes();
    std::cout << "\nRegistered function groups: " << groupTypes.size() << std::endl;
    
    // Test math functions
    std::cout << "\n=== Testing Math Functions ===" << std::endl;
    FunctionGroup* mathGroup = registry.getGroup(MATH_FUNCTIONS);
    if (mathGroup) {
        std::cout << "Group name: " << mathGroup->getName() << std::endl;
        
        // Test function names
        std::vector<std::string> funcNames = mathGroup->getFunctionNames();
        std::cout << "Available functions: ";
        for (size_t i = 0; i < funcNames.size(); ++i) {
            std::cout << funcNames[i];
            if (i < funcNames.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
        
        // Test add function (no parameters)
        if (mathGroup->hasFunction("add")) {
            void* result = mathGroup->callFunction("add");
            double value = *static_cast<double*>(result);
            std::cout << "add() = " << value << std::endl;
        }
        
        // Test sqrt function (one parameter)
        if (mathGroup->hasFunction("sqrt")) {
            double input = 16.0;
            void* result = mathGroup->callFunction("sqrt", &input);
            double value = *static_cast<double*>(result);
            std::cout << "sqrt(16) = " << value << std::endl;
        }
        
        // Test multiply function (using array for multiple values)
        if (mathGroup->hasFunction("multiply")) {
            double values[2] = {6.0, 7.0};
            void* result = mathGroup->callFunction("multiply", values);
            double value = *static_cast<double*>(result);
            std::cout << "multiply(6, 7) = " << value << std::endl;
        }
        
        // Test power function (two parameters)
        if (mathGroup->hasFunction("power")) {
            double base = 2.0;
            double exp = 3.0;
            void* args[2] = {&base, &exp};
            void* result = mathGroup->callFunction("power", args);
            double value = *static_cast<double*>(result);
            std::cout << "power(2, 3) = " << value << std::endl;
        }
    }
    
    // Test string functions
    std::cout << "\n=== Testing String Functions ===" << std::endl;
    FunctionGroup* stringGroup = registry.getGroup(STRING_FUNCTIONS);
    if (stringGroup) {
        std::cout << "Group name: " << stringGroup->getName() << std::endl;
        
        // Test function names
        std::vector<std::string> funcNames = stringGroup->getFunctionNames();
        std::cout << "Available functions: ";
        for (size_t i = 0; i < funcNames.size(); ++i) {
            std::cout << funcNames[i];
            if (i < funcNames.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
        
        // Test length function
        if (stringGroup->hasFunction("length")) {
            const char* input = "Hello World";
            void* result = stringGroup->callFunction("length", const_cast<char*>(input));
            int length = *static_cast<int*>(result);
            std::cout << "length(\"Hello World\") = " << length << std::endl;
        }
        
        // Test upper function
        if (stringGroup->hasFunction("upper")) {
            const char* input = "hello world";
            void* result = stringGroup->callFunction("upper", const_cast<char*>(input));
            const char* upper = static_cast<const char*>(result);
            std::cout << "upper(\"hello world\") = \"" << upper << "\"" << std::endl;
        }
        
        // Test reverse function
        if (stringGroup->hasFunction("reverse")) {
            const char* input = "hello";
            void* result = stringGroup->callFunction("reverse", const_cast<char*>(input));
            const char* reversed = static_cast<const char*>(result);
            std::cout << "reverse(\"hello\") = \"" << reversed << "\"" << std::endl;
        }
        
        // Test concat function
        if (stringGroup->hasFunction("concat")) {
            const char* str1 = "Hello ";
            const char* str2 = "World!";
            void* args[2] = {const_cast<char*>(str1), const_cast<char*>(str2)};
            void* result = stringGroup->callFunction("concat", args);
            const char* concatenated = static_cast<const char*>(result);
            std::cout << "concat(\"Hello \", \"World!\") = \"" << concatenated << "\"" << std::endl;
        }
    }
    
    // Test utility functions
    std::cout << "\n=== Testing Utility Functions ===" << std::endl;
    FunctionGroup* utilityGroup = registry.getGroup(UTILITY_FUNCTIONS);
    if (utilityGroup) {
        std::cout << "Group name: " << utilityGroup->getName() << std::endl;
        
        // Test function names
        std::vector<std::string> funcNames = utilityGroup->getFunctionNames();
        std::cout << "Available functions: ";
        for (size_t i = 0; i < funcNames.size(); ++i) {
            std::cout << funcNames[i];
            if (i < funcNames.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
        
        // Test random function
        if (utilityGroup->hasFunction("random")) {
            void* result = utilityGroup->callFunction("random");
            int value = *static_cast<int*>(result);
            std::cout << "random() = " << value << std::endl;
        }
        
        // Test factorial function
        if (utilityGroup->hasFunction("factorial")) {
            int input = 5;
            void* result = utilityGroup->callFunction("factorial", &input);
            int value = *static_cast<int*>(result);
            std::cout << "factorial(5) = " << value << std::endl;
        }
        
        // Test max function
        if (utilityGroup->hasFunction("max")) {
            int a = 15;
            int b = 23;
            void* args[2] = {&a, &b};
            void* result = utilityGroup->callFunction("max", args);
            int value = *static_cast<int*>(result);
            std::cout << "max(15, 23) = " << value << std::endl;
        }
    }
    
    std::cout << "\n=== Framework Demo Complete ===" << std::endl;
    return 0;
}
