#include "legacy_registry.h"

namespace Legacy {

// Static instance definition - now using unique_ptr
std::unique_ptr<LegacyRegistry> LegacyRegistry::instance = nullptr;

} // namespace Legacy
