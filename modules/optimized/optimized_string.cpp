#include "optimized_registry.h"
#include <cstring>
#include <cctype>

namespace Optimized {

// High-performance string functions using static buffers
const char* to_upper_fast(const char* str) {
    static thread_local char buffer[1024];
    const size_t len = std::strlen(str);
    const size_t copy_len = (len < 1023) ? len : 1023;
    
    for (size_t i = 0; i < copy_len; ++i) {
        buffer[i] = static_cast<char>(std::toupper(str[i]));
    }
    buffer[copy_len] = '\0';
    return buffer;
}

const char* to_lower_fast(const char* str) {
    static thread_local char buffer[1024];
    const size_t len = std::strlen(str);
    const size_t copy_len = (len < 1023) ? len : 1023;
    
    for (size_t i = 0; i < copy_len; ++i) {
        buffer[i] = static_cast<char>(std::tolower(str[i]));
    }
    buffer[copy_len] = '\0';
    return buffer;
}

int string_length_fast(const char* str) {
    return static_cast<int>(std::strlen(str));
}

const char* reverse_string_fast(const char* str) {
    static thread_local char buffer[1024];
    const int len = static_cast<int>(std::strlen(str));
    const int copy_len = (len < 1023) ? len : 1023;
    
    for (int i = 0; i < copy_len; ++i) {
        buffer[i] = str[copy_len - 1 - i];
    }
    buffer[copy_len] = '\0';
    return buffer;
}

// Fast character counting
int count_chars_fast(const char* str) {
    int count = 0;
    while (*str++) {
        ++count;
    }
    return count;
}

// Auto-registration class
class OptimizedStringRegistrar {
public:
    OptimizedStringRegistrar() {
        // Register the string group
        OPTIMIZED_REGISTER_GROUP(FunctionCategory::STRING, "Optimized String Functions");
        
        // Register individual functions
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::STRING, "to_upper", to_upper_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::STRING, "to_lower", to_lower_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::STRING, "length", string_length_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::STRING, "reverse", reverse_string_fast);
        OPTIMIZED_REGISTER_FUNCTION(FunctionCategory::STRING, "count_chars", count_chars_fast);
    }
};

// Static instance to trigger registration
static OptimizedStringRegistrar stringRegistrar;

} // namespace Optimized
