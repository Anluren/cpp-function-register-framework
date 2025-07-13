#include "function_registry.h"
#include <algorithm>

// Initialize static instance
FunctionRegistry* FunctionRegistry::instance = 0;

FunctionRegistry& FunctionRegistry::getInstance() {
    if (instance == 0) {
        instance = new FunctionRegistry();
    }
    return *instance;
}

FunctionRegistry::~FunctionRegistry() {
    clear();
}

void FunctionRegistry::registerGroup(FunctionGroupType type, FunctionGroup* group) {
    if (group != 0) {
        // If group already exists, delete the old one
        if (groups.find(type) != groups.end()) {
            delete groups[type];
        }
        groups[type] = group;
    }
}

FunctionGroup* FunctionRegistry::getGroup(FunctionGroupType type) {
    std::map<FunctionGroupType, FunctionGroup*>::iterator it = groups.find(type);
    if (it != groups.end()) {
        return it->second;
    }
    return 0;
}

bool FunctionRegistry::hasGroup(FunctionGroupType type) const {
    return groups.find(type) != groups.end();
}

std::vector<FunctionGroupType> FunctionRegistry::getGroupTypes() const {
    std::vector<FunctionGroupType> types;
    for (std::map<FunctionGroupType, FunctionGroup*>::const_iterator it = groups.begin();
         it != groups.end(); ++it) {
        types.push_back(it->first);
    }
    return types;
}

void FunctionRegistry::unregisterGroup(FunctionGroupType type) {
    std::map<FunctionGroupType, FunctionGroup*>::iterator it = groups.find(type);
    if (it != groups.end()) {
        delete it->second;
        groups.erase(it);
    }
}

void FunctionRegistry::clear() {
    for (std::map<FunctionGroupType, FunctionGroup*>::iterator it = groups.begin();
         it != groups.end(); ++it) {
        delete it->second;
    }
    groups.clear();
}
