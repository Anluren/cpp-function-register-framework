#include "legacy_registry.h"
#include <cstring>
#include <algorithm>
#include <cctype>

namespace Legacy {

// String functions for legacy module
const char* to_upper_legacy(const char* str) {
    static char buffer[256];
    strncpy(buffer, str, 255);
    buffer[255] = '\0';
    
    for (int i = 0; buffer[i]; i++) {
        buffer[i] = std::toupper(buffer[i]);
    }
    return buffer;
}

const char* to_lower_legacy(const char* str) {
    static char buffer[256];
    strncpy(buffer, str, 255);
    buffer[255] = '\0';
    
    for (int i = 0; buffer[i]; i++) {
        buffer[i] = std::tolower(buffer[i]);
    }
    return buffer;
}

int string_length_legacy(const char* str) {
    return static_cast<int>(strlen(str));
}

const char* reverse_string_legacy(const char* str) {
    static char buffer[256];
    int len = strlen(str);
    if (len > 255) len = 255;
    
    for (int i = 0; i < len; i++) {
        buffer[i] = str[len - 1 - i];
    }
    buffer[len] = '\0';
    return buffer;
}

// Auto-registration using static initialization
class LegacyStringRegistrar {
public:
    LegacyStringRegistrar() {
        // Register the string group
        LEGACY_REGISTER_GROUP(STRING, "Legacy String Functions");
        
        // Register individual functions
        LEGACY_REGISTER_FUNCTION(STRING, "to_upper", to_upper_legacy);
        LEGACY_REGISTER_FUNCTION(STRING, "to_lower", to_lower_legacy);
        LEGACY_REGISTER_FUNCTION(STRING, "length", string_length_legacy);
        LEGACY_REGISTER_FUNCTION(STRING, "reverse", reverse_string_legacy);
    }
};

// Static instance to trigger registration
static LegacyStringRegistrar stringRegistrar;

// Function to force linking
void force_string_registration() {
    // This function forces the static registrar to be linked
    static bool initialized = false;
    if (!initialized) {
        // Touch the static variable to ensure it gets constructed
        (void)&stringRegistrar;
        initialized = true;
    }
}

} // namespace Legacy
