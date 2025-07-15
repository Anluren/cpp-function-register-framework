#include "modern_registry.h"
#include <mutex>

namespace Modern {

// Static member definitions
std::unique_ptr<ModernRegistry> ModernRegistry::instance = nullptr;
std::once_flag ModernRegistry::initFlag;

// Forward declarations for module initialization
void initMathFunctions();
void initStringFunctions();
void initUtilityFunctions();

// Initialize all modules
void ModernRegistry::initializeModules() {
    initMathFunctions();
    initStringFunctions();
    initUtilityFunctions();
}

} // namespace Modern
