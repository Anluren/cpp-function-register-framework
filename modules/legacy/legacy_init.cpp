#include "legacy_registry.h"

namespace Legacy {

// Forward declarations to force linking of static registrars
extern void force_math_registration();
extern void force_string_registration();
extern void force_utility_registration();

// Function to force initialization of all modules
void initializeLegacyModules() {
    force_math_registration();
    force_string_registration();
    force_utility_registration();
}

} // namespace Legacy
