#include <iostream>
#include <string_view>
#include <array>

/**
 * @file compile_time_strings.cpp
 * @brief Demonstrates various approaches to create compile-time strings as static class members
 */

// Approach 1: Using constexpr string_view (C++17) - Most common and efficient
class StringViewExample {
public:
    static constexpr std::string_view name = "StringViewExample";
    static constexpr std::string_view description = "Using std::string_view for compile-time strings";
    static constexpr std::string_view version = "1.0.0";
    
    void printInfo() const {
        std::cout << "Class: " << name << std::endl;
        std::cout << "Description: " << description << std::endl;
        std::cout << "Version: " << version << std::endl;
    }
};

// Approach 2: Using constexpr const char* - Traditional approach
class CharPtrExample {
public:
    static constexpr const char* name = "CharPtrExample";
    static constexpr const char* description = "Using const char* for compile-time strings";
    static constexpr const char* version = "2.0.0";
    
    void printInfo() const {
        std::cout << "Class: " << name << std::endl;
        std::cout << "Description: " << description << std::endl;
        std::cout << "Version: " << version << std::endl;
    }
};

// Approach 3: Using constexpr std::array<char, N> - When you need modification
template<size_t N>
class ArrayExample {
public:
    static constexpr std::array<char, 16> name = {'A', 'r', 'r', 'a', 'y', 'E', 'x', 'a', 'm', 'p', 'l', 'e', '\0'};
    static constexpr std::array<char, 32> description = {'U', 's', 'i', 'n', 'g', ' ', 'a', 'r', 'r', 'a', 'y', '\0'};
    
    void printInfo() const {
        std::cout << "Class: " << name.data() << std::endl;
        std::cout << "Description: " << description.data() << std::endl;
    }
};

// Approach 4: Template-based compile-time string with helper
template<char... chars>
struct CompileTimeString {
    static constexpr char value[] = {chars..., '\0'};
    static constexpr size_t size = sizeof...(chars);
    static constexpr std::string_view view() { return {value, size}; }
};

template<char... chars>
constexpr char CompileTimeString<chars...>::value[];

// Helper macro for easier usage
#define MAKE_STRING(str) []() { \
    constexpr auto s = str; \
    return s; \
}()

class TemplateStringExample {
public:
    static constexpr auto name = MAKE_STRING("TemplateStringExample");
    static constexpr auto description = MAKE_STRING("Template-based compile-time string");
    
    void printInfo() const {
        std::cout << "Class: " << name << std::endl;
        std::cout << "Description: " << description << std::endl;
    }
};

// Approach 5: Using inline constexpr for C++17 (avoids ODR issues)
class InlineExample {
public:
    inline static constexpr std::string_view name = "InlineExample";
    inline static constexpr std::string_view description = "Using inline constexpr for safer static members";
    
    void printInfo() const {
        std::cout << "Class: " << name << std::endl;
        std::cout << "Description: " << description << std::endl;
    }
};

// Approach 6: Using consteval (C++20) for guaranteed compile-time evaluation
#if __cplusplus >= 202002L
consteval const char* make_compile_time_string(const char* str) {
    return str;
}

class ConstevalExample {
public:
    static constexpr auto name = make_compile_time_string("ConstevalExample");
    static constexpr auto description = make_compile_time_string("Using consteval for C++20");
    
    void printInfo() const {
        std::cout << "Class: " << name << std::endl;
        std::cout << "Description: " << description << std::endl;
    }
};
#endif

// Approach 7: Enum class with constexpr conversion for type-safe string constants
enum class StringId {
    NAME,
    DESCRIPTION,
    VERSION
};

class EnumStringExample {
public:
    static constexpr std::string_view name = "EnumStringExample";
    static constexpr std::string_view description = "Type-safe string constants";
    static constexpr std::string_view version = "3.0.0";
    
    void printInfo() const {
        std::cout << "Class: " << name << std::endl;
        std::cout << "Description: " << description << std::endl;
        std::cout << "Version: " << version << std::endl;
    }
};

// Approach 8: Function registry example with compile-time strings
class FunctionRegistry {
public:
    struct FunctionInfo {
        std::string_view name;
        std::string_view category;
        std::string_view description;
    };
    
    // Compile-time function metadata
    static constexpr FunctionInfo mathFunctions[] = {
        {"add", "math", "Addition operation"},
        {"subtract", "math", "Subtraction operation"},
        {"multiply", "math", "Multiplication operation"},
        {"divide", "math", "Division operation"}
    };
    
    static constexpr FunctionInfo stringFunctions[] = {
        {"concat", "string", "String concatenation"},
        {"length", "string", "String length calculation"},
        {"reverse", "string", "String reversal"}
    };
    
    static constexpr size_t mathFunctionCount = sizeof(mathFunctions) / sizeof(FunctionInfo);
    static constexpr size_t stringFunctionCount = sizeof(stringFunctions) / sizeof(FunctionInfo);
    
    void printAllFunctions() const {
        std::cout << "\n=== Math Functions ===" << std::endl;
        for (size_t i = 0; i < mathFunctionCount; ++i) {
            std::cout << mathFunctions[i].name << ": " << mathFunctions[i].description << std::endl;
        }
        
        std::cout << "\n=== String Functions ===" << std::endl;
        for (size_t i = 0; i < stringFunctionCount; ++i) {
            std::cout << stringFunctions[i].name << ": " << stringFunctions[i].description << std::endl;
        }
    }
};

// Static member definitions (only needed for pre-C++17 or when not using inline)
constexpr FunctionRegistry::FunctionInfo FunctionRegistry::mathFunctions[];
constexpr FunctionRegistry::FunctionInfo FunctionRegistry::stringFunctions[];

int main() {
    std::cout << "Compile-Time String Examples" << std::endl;
    std::cout << "============================" << std::endl;
    
    std::cout << "\n1. String View Example:" << std::endl;
    StringViewExample().printInfo();
    
    std::cout << "\n2. Char Pointer Example:" << std::endl;
    CharPtrExample().printInfo();
    
    std::cout << "\n3. Array Example:" << std::endl;
    ArrayExample<16>().printInfo();
    
    std::cout << "\n4. Template String Example:" << std::endl;
    TemplateStringExample().printInfo();
    
    std::cout << "\n5. Inline Example:" << std::endl;
    InlineExample().printInfo();
    
#if __cplusplus >= 202002L
    std::cout << "\n6. Consteval Example (C++20):" << std::endl;
    ConstevalExample().printInfo();
#endif
    
    std::cout << "\n7. Enum String Example:" << std::endl;
    EnumStringExample().printInfo();
    
    std::cout << "\n8. Function Registry Example:" << std::endl;
    FunctionRegistry().printAllFunctions();
    
    // Demonstrate compile-time properties
    std::cout << "\n=== Compile-Time Properties ===" << std::endl;
    std::cout << "StringViewExample::name length: " << StringViewExample::name.length() << std::endl;
    std::cout << "Can be used in constexpr context: " << (StringViewExample::name == "StringViewExample" ? "Yes" : "No") << std::endl;
    
    // Show that these are truly compile-time constants
    static_assert(StringViewExample::name == "StringViewExample");
    static_assert(CharPtrExample::name[0] == 'C');
    static_assert(InlineExample::name.length() == 13);
    
    std::cout << "\nAll static assertions passed - strings are compile-time constants!" << std::endl;
    
    return 0;
}
