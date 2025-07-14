#include "enhanced_function_group_cpp98.h"
#include <iostream>
#include <cmath>

// Example enhanced functions with better type information
void* enhanced_add() {
    static double result = 10.0 + 5.0;
    return &result;
}

void* enhanced_multiply(void* a_ptr) {
    // Expects an array of two doubles
    double* values = static_cast<double*>(a_ptr);
    static double result = values[0] * values[1];
    return &result;
}

void* enhanced_power(void* base_ptr, void* exp_ptr) {
    double base = *static_cast<double*>(base_ptr);
    double exp = *static_cast<double*>(exp_ptr);
    static double result = pow(base, exp);
    return &result;
}

void* enhanced_sum_three(void* a_ptr, void* b_ptr, void* c_ptr) {
    double a = *static_cast<double*>(a_ptr);
    double b = *static_cast<double*>(b_ptr);
    double c = *static_cast<double*>(c_ptr);
    static double result = a + b + c;
    return &result;
}

void* enhanced_average_four(void* a_ptr, void* b_ptr, void* c_ptr, void* d_ptr) {
    double a = *static_cast<double*>(a_ptr);
    double b = *static_cast<double*>(b_ptr);
    double c = *static_cast<double*>(c_ptr);
    double d = *static_cast<double*>(d_ptr);
    static double result = (a + b + c + d) / 4.0;
    return &result;
}

int main() {
    std::cout << "=== Enhanced Function Group Demo ===" << std::endl;
    
    // Create enhanced function group with metadata
    EnhancedFunctionGroup enhancedMath("Enhanced Math Functions", 
                                      "Mathematical operations with enhanced type safety", 
                                      "2.0.0");
    
    // Register functions with detailed metadata
    enhancedMath.registerFunction("add", enhanced_add, 
                                  "Adds 10 + 5", "double");
    
    enhancedMath.registerFunction("multiply", enhanced_multiply,
                                 "Multiplies two numbers from array", "double",
                                 "double[2]", "values");
    
    enhancedMath.registerFunction("power", enhanced_power,
                                 "Raises base to the power of exponent", "double",
                                 "double", "base", "double", "exponent");
    
    enhancedMath.registerFunction("sum_three", enhanced_sum_three,
                                 "Sums three numbers", "double",
                                 "double", "a", "double", "b", "double", "c");
    
    enhancedMath.registerFunction("average_four", enhanced_average_four,
                                 "Calculates average of four numbers", "double",
                                 "double", "a", "double", "b", "double", "c", "double", "d");
    
    // Display group information
    std::cout << "\nGroup: " << enhancedMath.getName() << std::endl;
    std::cout << "Description: " << enhancedMath.getDescription() << std::endl;
    std::cout << "Version: " << enhancedMath.getVersion() << std::endl;
    
    // Display function information
    std::cout << "\nAvailable Functions:" << std::endl;
    std::vector<FunctionInfo> infos = enhancedMath.getFunctionInfos();
    for (size_t i = 0; i < infos.size(); ++i) {
        const FunctionInfo& info = infos[i];
        std::cout << "  " << info.name << ": " << info.description << std::endl;
        std::cout << "    Returns: " << info.returnType << std::endl;
        std::cout << "    Parameters (" << info.parameterCount << "): ";
        for (int j = 0; j < info.parameterCount; ++j) {
            if (j > 0) std::cout << ", ";
            std::cout << info.parameterTypes[j] << " " << info.parameterNames[j];
        }
        std::cout << std::endl << std::endl;
    }
    
    // Test function calls with enhanced error handling
    std::cout << "=== Function Call Tests ===" << std::endl;
    
    // Test 0-parameter function
    std::cout << "\nTesting add():" << std::endl;
    FunctionCallResult result = enhancedMath.callFunction("add");
    if (result.isSuccess()) {
        double value = result.getValue<double>();
        std::cout << "Result: " << value << " (type: " << result.getReturnType() << ")" << std::endl;
    } else {
        std::cout << "Error: " << result.getError() << std::endl;
    }
    
    // Test 1-parameter function
    std::cout << "\nTesting multiply(6.0, 7.0):" << std::endl;
    double values[2] = {6.0, 7.0};
    result = enhancedMath.callFunction("multiply", values);
    if (result.isSuccess()) {
        double value = result.getValue<double>();
        std::cout << "Result: " << value << " (type: " << result.getReturnType() << ")" << std::endl;
    } else {
        std::cout << "Error: " << result.getError() << std::endl;
    }
    
    // Test 2-parameter function
    std::cout << "\nTesting power(2.0, 3.0):" << std::endl;
    double base = 2.0;
    double exp = 3.0;
    result = enhancedMath.callFunction("power", &base, &exp);
    if (result.isSuccess()) {
        double value = result.getValue<double>();
        std::cout << "Result: " << value << " (type: " << result.getReturnType() << ")" << std::endl;
    } else {
        std::cout << "Error: " << result.getError() << std::endl;
    }
    
    // Test 3-parameter function
    std::cout << "\nTesting sum_three(1.5, 2.5, 3.0):" << std::endl;
    double a = 1.5, b = 2.5, c = 3.0;
    result = enhancedMath.callFunction("sum_three", &a, &b, &c);
    if (result.isSuccess()) {
        double value = result.getValue<double>();
        std::cout << "Result: " << value << " (type: " << result.getReturnType() << ")" << std::endl;
    } else {
        std::cout << "Error: " << result.getError() << std::endl;
    }
    
    // Test 4-parameter function
    std::cout << "\nTesting average_four(10, 20, 30, 40):" << std::endl;
    double v1 = 10.0, v2 = 20.0, v3 = 30.0, v4 = 40.0;
    result = enhancedMath.callFunction("average_four", &v1, &v2, &v3, &v4);
    if (result.isSuccess()) {
        double value = result.getValue<double>();
        std::cout << "Result: " << value << " (type: " << result.getReturnType() << ")" << std::endl;
    } else {
        std::cout << "Error: " << result.getError() << std::endl;
    }
    
    // Test error handling - wrong parameter count
    std::cout << "\nTesting error handling - calling power() with no parameters:" << std::endl;
    result = enhancedMath.callFunction("power");
    if (result.isSuccess()) {
        double value = result.getValue<double>();
        std::cout << "Result: " << value << std::endl;
    } else {
        std::cout << "Error (expected): " << result.getError() << std::endl;
    }
    
    // Test error handling - function not found
    std::cout << "\nTesting error handling - calling non-existent function:" << std::endl;
    result = enhancedMath.callFunction("nonexistent");
    if (result.isSuccess()) {
        std::cout << "Unexpected success!" << std::endl;
    } else {
        std::cout << "Error (expected): " << result.getError() << std::endl;
    }
    
    std::cout << "\n=== Enhanced Function Group Demo Complete ===" << std::endl;
    return 0;
}
