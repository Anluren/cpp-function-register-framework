#include "legacy_registry.h"
#include <ctime>
#include <cstdlib>

namespace Legacy {

// Utility functions for legacy module
int random_number_legacy() {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned int>(time(0)));
        seeded = true;
    }
    return rand();
}

int random_range_legacy(int min, int max) {
    if (max <= min) return min;
    return min + (random_number_legacy() % (max - min + 1));
}

int current_time_legacy() {
    return static_cast<int>(time(0));
}

const char* get_version_legacy() {
    return "Legacy Function Registry v1.0";
}

int factorial_legacy(int n) {
    if (n <= 1) return 1;
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

// Auto-registration using static initialization
class LegacyUtilityRegistrar {
public:
    LegacyUtilityRegistrar() {
        // Register the utility group
        LEGACY_REGISTER_GROUP(UTILITY, "Legacy Utility Functions");
        
        // Register individual functions
        LEGACY_REGISTER_FUNCTION(UTILITY, "random", random_number_legacy);
        LEGACY_REGISTER_FUNCTION(UTILITY, "random_range", random_range_legacy);
        LEGACY_REGISTER_FUNCTION(UTILITY, "current_time", current_time_legacy);
        LEGACY_REGISTER_FUNCTION(UTILITY, "version", get_version_legacy);
        LEGACY_REGISTER_FUNCTION(UTILITY, "factorial", factorial_legacy);
    }
};

// Static instance to trigger registration
static LegacyUtilityRegistrar utilityRegistrar;

// Function to force linking
void force_utility_registration() {
    // This function forces the static registrar to be linked
    static bool initialized = false;
    if (!initialized) {
        // Touch the static variable to ensure it gets constructed
        (void)&utilityRegistrar;
        initialized = true;
    }
}

} // namespace Legacy
