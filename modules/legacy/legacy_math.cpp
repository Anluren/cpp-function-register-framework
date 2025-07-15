#include "legacy_registry.h"
#include <cmath>

namespace Legacy {

// Math functions for legacy module
int add_legacy(int a, int b) {
    return a + b;
}

int subtract_legacy(int a, int b) {
    return a - b;
}

int multiply_legacy(int a, int b) {
    return a * b;
}

int divide_legacy(int a, int b) {
    return (b != 0) ? a / b : 0;
}

double sqrt_legacy(double x) {
    return std::sqrt(x);
}

double power_legacy(double base, double exp) {
    return std::pow(base, exp);
}

// Auto-registration using static initialization
class LegacyMathRegistrar {
public:
    LegacyMathRegistrar() {
        // Register the math group
        LEGACY_REGISTER_GROUP(MATH, "Legacy Math Functions");
        
        // Register individual functions
        LEGACY_REGISTER_FUNCTION(MATH, "add", add_legacy);
        LEGACY_REGISTER_FUNCTION(MATH, "subtract", subtract_legacy);
        LEGACY_REGISTER_FUNCTION(MATH, "multiply", multiply_legacy);
        LEGACY_REGISTER_FUNCTION(MATH, "divide", divide_legacy);
        LEGACY_REGISTER_FUNCTION(MATH, "sqrt", sqrt_legacy);
        LEGACY_REGISTER_FUNCTION(MATH, "power", power_legacy);
    }
};

// Static instance to trigger registration
static LegacyMathRegistrar mathRegistrar;

// Function to force linking
void force_math_registration() {
    // This function forces the static registrar to be linked
    static bool initialized = false;
    if (!initialized) {
        // Touch the static variable to ensure it gets constructed
        (void)&mathRegistrar;
        initialized = true;
    }
}

} // namespace Legacy
