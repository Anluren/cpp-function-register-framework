#pragma once

#include "../../include/multi_module_registry.h"

/**
 * Multi-Module Registry Module
 * 
 * Supports multiple modules implementing the same function ID,
 * with caller-controlled selection of which implementation to use.
 */

namespace MultiModuleRegistry {
    
    // Initialize all multi-module implementations
    void initialize_multi_module_registry();
    
    // Individual module initialization functions
    void init_basic_math_module();
    void init_optimized_math_module();
    void init_debug_math_module();
    
}
