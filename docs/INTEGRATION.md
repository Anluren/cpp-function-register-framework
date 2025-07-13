# Integration Example

This directory contains an example of how to integrate the Function Register Framework into your own CMake project.

## Using the Framework

### Method 1: As a Git Submodule

```bash
# Add as submodule
git submodule add <repository-url> third_party/func_register
git submodule update --init --recursive

# In your CMakeLists.txt
add_subdirectory(third_party/func_register)
target_link_libraries(your_target FunctionRegisterFramework::function_register)
```

### Method 2: Installed Package

```bash
# Build and install the framework
cd func_register
./build.sh --install

# In your CMakeLists.txt
find_package(FunctionRegisterFramework REQUIRED)
target_link_libraries(your_target FunctionRegisterFramework::function_register)
```

### Method 3: Manual Integration

```cmake
# Add include directory
target_include_directories(your_target PRIVATE /path/to/func_register/include)

# Link library
target_link_libraries(your_target /path/to/func_register/build/src/libfunction_register.a)
```

## Example Integration

```cpp
// my_project.cpp
#include "function_registry.h"
#include <iostream>

// Your custom functions
void* database_connect() {
    // Database connection logic
    static bool connected = true;
    std::cout << "Database connected!" << std::endl;
    return &connected;
}

void* database_query(void* sql_ptr) {
    const char* sql = static_cast<const char*>(sql_ptr);
    std::cout << "Executing: " << sql << std::endl;
    static int rows = 42;
    return &rows;
}

// Register your functions
void initDatabaseFunctions() {
    ConcreteFunctionGroup* dbGroup = new ConcreteFunctionGroup("Database Functions");
    
    dbGroup->registerFunction0("connect", database_connect);
    dbGroup->registerFunction1("query", database_query);
    
    // Assuming you added DATABASE_FUNCTIONS to the enum
    FunctionRegistry::getInstance().registerGroup(CUSTOM_GROUP_1, dbGroup);
}

// Auto-register
static class DatabaseInitializer {
public:
    DatabaseInitializer() { initDatabaseFunctions(); }
} dbInit;

int main() {
    FunctionRegistry& registry = FunctionRegistry::getInstance();
    FunctionGroup* db = registry.getGroup(CUSTOM_GROUP_1);
    
    if (db) {
        // Connect to database
        db->callFunction("connect");
        
        // Execute query
        const char* sql = "SELECT * FROM users";
        void* result = db->callFunction("query", const_cast<char*>(sql));
        int rows = *static_cast<int*>(result);
        std::cout << "Query returned " << rows << " rows" << std::endl;
    }
    
    return 0;
}
```

## CMakeLists.txt Example

```cmake
cmake_minimum_required(VERSION 3.10)
project(MyProject)

set(CMAKE_CXX_STANDARD 11)

# Method 1: Using submodule
add_subdirectory(third_party/func_register)
add_executable(my_project my_project.cpp)
target_link_libraries(my_project FunctionRegisterFramework::function_register)

# Method 2: Using installed package
# find_package(FunctionRegisterFramework REQUIRED)
# add_executable(my_project my_project.cpp)
# target_link_libraries(my_project FunctionRegisterFramework::function_register)
```

## Directory Structure

```
your_project/
├── CMakeLists.txt
├── src/
│   └── my_project.cpp
└── third_party/          # If using submodule
    └── func_register/
        ├── include/
        ├── src/
        └── CMakeLists.txt
```
