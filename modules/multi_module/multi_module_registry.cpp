#include "multi_module_registry.h"
#include <iostream>

namespace MultiModuleRegistry {
    void initialize_multi_module_registry() {
        std::cout << "Multi-module registry initialized" << std::endl;
        init_basic_math_module();
        init_optimized_math_module();
        init_debug_math_module();
    }
    
    void init_basic_math_module() {
        std::cout << "Basic math module initialized" << std::endl;
    }
    
    void init_optimized_math_module() {
        std::cout << "Optimized math module initialized" << std::endl;
    }
    
    void init_debug_math_module() {
        std::cout << "Debug math module initialized" << std::endl;
    }
}
