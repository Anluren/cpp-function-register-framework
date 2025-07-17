#include <iostream>
#include <string>
#include <type_traits> // For std::true_type, std::false_type, std::is_same

// Your SFINAE template
template <typename T, typename Ret, typename... Args>
struct has_member_function_foo {
private:
    // SFINAE-friendly overload that succeeds if T has 'foo' with the correct signature
    template <typename U>
    static auto check(U*) -> decltype(std::declval<U>().foo(std::declval<Args>()...), std::true_type{});

    // Fallback overload for when the above fails
    template <typename>
    static std::false_type check(...);

    // Check if the return type matches
    template <typename U>
    static constexpr bool return_type_matches() {
        if constexpr (std::is_same_v<decltype(check<U>(nullptr)), std::true_type>) {
            return std::is_same_v<decltype(std::declval<U>().foo(std::declval<Args>()...)), Ret>;
        } else {
            return false;
        }
    }

public:
    // Value indicating whether the member function exists with correct signature
    static constexpr bool value = return_type_matches<T>();
};

// Helper variable template for convenience (C++14 style)
template <typename T, typename Ret, typename... Args>
constexpr bool has_member_function_foo_v = has_member_function_foo<T, Ret, Args...>::value;

// Test classes
class ClassWithFoo {
public:
    int foo(int x, double y) {
        return static_cast<int>(x + y);
    }
    
    std::string foo(const std::string& s) {
        return "foo: " + s;
    }
    
    void foo() {
        std::cout << "foo() called\n";
    }
};

class ClassWithoutFoo {
public:
    int bar(int x) {
        return x * 2;
    }
};

class ClassWithWrongFoo {
public:
    // Wrong signature - takes string instead of int, double
    int foo(const std::string& s) {
        return s.length();
    }
};

class ClassWithFooWrongReturn {
public:
    // Wrong return type - returns string instead of int
    std::string foo(int x, double y) {
        return std::to_string(x + y);
    }
};

// Test function template
template<typename T, typename Ret, typename... Args>
void test_detection(const std::string& class_name, const std::string& signature) {
    constexpr bool has_foo = has_member_function_foo_v<T, Ret, Args...>;
    std::cout << class_name << " has foo" << signature << ": " 
              << (has_foo ? "YES" : "NO") << std::endl;
}

int main() {
    std::cout << "=== Testing SFINAE Member Function Detection ===\n\n";
    
    // Test 1: ClassWithFoo - should have int foo(int, double)
    test_detection<ClassWithFoo, int, int, double>("ClassWithFoo", "(int, double) -> int");
    
    // Test 2: ClassWithFoo - should have std::string foo(const std::string&)
    test_detection<ClassWithFoo, std::string, const std::string&>("ClassWithFoo", "(const std::string&) -> std::string");
    
    // Test 3: ClassWithFoo - should have void foo()
    test_detection<ClassWithFoo, void>("ClassWithFoo", "() -> void");
    
    // Test 4: ClassWithoutFoo - should NOT have any foo
    test_detection<ClassWithoutFoo, int, int, double>("ClassWithoutFoo", "(int, double) -> int");
    
    // Test 5: ClassWithWrongFoo - should NOT have int foo(int, double)
    test_detection<ClassWithWrongFoo, int, int, double>("ClassWithWrongFoo", "(int, double) -> int");
    
    // Test 6: ClassWithWrongFoo - should have int foo(const std::string&)
    test_detection<ClassWithWrongFoo, int, const std::string&>("ClassWithWrongFoo", "(const std::string&) -> int");
    
    // Test 7: ClassWithFooWrongReturn - should NOT have int foo(int, double) (wrong return type)
    test_detection<ClassWithFooWrongReturn, int, int, double>("ClassWithFooWrongReturn", "(int, double) -> int");
    
    // Test 8: ClassWithFooWrongReturn - should have std::string foo(int, double)
    test_detection<ClassWithFooWrongReturn, std::string, int, double>("ClassWithFooWrongReturn", "(int, double) -> std::string");
    
    std::cout << "\n=== Testing with actual function calls ===\n";
    
    // Demonstrate actual usage
    ClassWithFoo obj;
    
    if constexpr (has_member_function_foo_v<ClassWithFoo, int, int, double>) {
        std::cout << "Calling obj.foo(10, 3.5): " << obj.foo(10, 3.5) << std::endl;
    }
    
    if constexpr (has_member_function_foo_v<ClassWithFoo, std::string, const std::string&>) {
        std::cout << "Calling obj.foo(\"test\"): " << obj.foo(std::string("test")) << std::endl;
    }
    
    if constexpr (has_member_function_foo_v<ClassWithFoo, void>) {
        std::cout << "Calling obj.foo(): ";
        obj.foo();
    }
    
    // This would fail to compile if uncommented (which proves the detection works)
    /*
    if constexpr (has_member_function_foo_v<ClassWithoutFoo, int, int, double>) {
        ClassWithoutFoo obj2;
        obj2.foo(1, 2.0);  // Compile error - foo doesn't exist
    }
    */
    
    std::cout << "\n=== Testing edge cases ===\n";
    
    // Test with const member functions
    class ClassWithConstFoo {
    public:
        int foo(int x) const { return x * 2; }
    };
    
    test_detection<ClassWithConstFoo, int, int>("ClassWithConstFoo", "(int) -> int");
    
    // Test with static member functions
    class ClassWithStaticFoo {
    public:
        static int foo(int x) { return x * 3; }
    };
    
    test_detection<ClassWithStaticFoo, int, int>("ClassWithStaticFoo", "(int) -> int");
    
    return 0;
}
