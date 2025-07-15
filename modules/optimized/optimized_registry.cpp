#include "optimized_registry.h"
#include "optimized_math_functions.h"
#include "optimized_string_functions.h"
#include "optimized_utility_functions.h"

namespace Optimized {

// Static instance definition
std::unique_ptr<OptimizedRegistry> OptimizedRegistry::instance = nullptr;

// Initialize all modules
    void initializeOptimizedModule() {
    Optimized::initMathFunctions();
    Optimized::initStringFunctions();
    Optimized::initUtilityFunctions();
}
} // namespace Optimized
