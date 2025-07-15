#include "modern_function_group.h"
#include "modern_math_functions.h"
#include "modern_string_functions.h" 
#include "modern_utility_functions.h"

namespace ModernModule {
    
    void initialize_modern_functions() {
        init_math_functions();
        init_string_functions();
        init_utility_functions();
    }
    
}
