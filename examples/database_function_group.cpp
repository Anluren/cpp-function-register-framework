#include "simple_modern_function_group.h"
#include <iostream>
#include <cmath>

// Example: Database-related functions
// This demonstrates how to create a completely self-contained function group

namespace database_functions {

// Function implementations
bool connect_db(const std::string& connection_string) {
    std::cout << "Connecting to database: " << connection_string << std::endl;
    return true; // Simulate successful connection
}

int query_count(const std::string& table_name) {
    std::cout << "Counting records in table: " << table_name << std::endl;
    return 42; // Simulate record count
}

std::string get_user_name(int user_id) {
    return "User_" + std::to_string(user_id);
}

void log_operation(const std::string& operation) {
    std::cout << "[DB LOG] " << operation << std::endl;
}

double calculate_avg_score() {
    return 85.7;
}

// Registration function
void register_database_functions() {
    auto db_group = std::make_unique<SimpleFunctionGroup>("Database Operations");
    
    // Register all database functions
    db_group->add("connect", connect_db);
    db_group->add("count", query_count);
    db_group->add("get_user", get_user_name);
    db_group->add("log", log_operation);
    db_group->add("avg_score", calculate_avg_score);
    
    // Register lambdas for utility operations
    db_group->add("format_connection", [](const std::string& host, const std::string& db) {
        return "host=" + host + ";database=" + db;
    });
    
    db_group->add("validate_id", [](int id) {
        return id > 0 && id < 10000;
    });
    
    // Register with global registry
    auto& registry = SimpleFunctionRegistry::instance();
    registry.register_group(FunctionGroupType::CUSTOM_GROUP_1, std::move(db_group));
    
    std::cout << "Database functions registered automatically!" << std::endl;
}

} // namespace database_functions

// Automatic registration using static initialization
// This ensures the function group is registered when the module is loaded
namespace {
    struct DatabaseFunctionAutoRegistration {
        DatabaseFunctionAutoRegistration() {
            database_functions::register_database_functions();
        }
    };
    
    // Static instance triggers registration at program startup
    // The variable name doesn't matter - it just needs to exist
    static DatabaseFunctionAutoRegistration auto_register_db_functions;
}

/*
 * USAGE NOTES:
 * 
 * 1. Self-Contained Module:
 *    - All function implementations are in this file
 *    - Registration logic is in this file  
 *    - Automatic registration happens at program startup
 *    - No manual calls needed from main()
 * 
 * 2. To use this module:
 *    - Simply link this .cpp file to your project
 *    - Functions are automatically available through the registry
 *    - No #include needed in main() (just link the object file)
 * 
 * 3. Access functions like this:
 *    auto& registry = SimpleFunctionRegistry::instance();
 *    auto* db_group = registry.get_group(FunctionGroupType::CUSTOM_GROUP_1);
 *    auto result = db_group->call_as<bool>("connect", "localhost");
 * 
 * 4. Benefits:
 *    - Modular: Each functionality is in its own file
 *    - Self-registering: No manual setup required
 *    - Maintainable: Functions and registration are co-located
 *    - Scalable: Easy to add new function groups
 */
