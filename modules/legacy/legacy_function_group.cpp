#include "legacy_function_group.h"
#include "legacy_math_functions.h"
#include "legacy_string_functions.h"
#include "legacy_utility_functions.h"

namespace LegacyModule {
    
    void initialize_legacy_functions() {
        init_math_functions();
        init_string_functions();
        init_utility_functions();
    }
    
}
