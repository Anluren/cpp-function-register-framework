#include "modern_registry.h"
#include <algorithm>
#include <sstream>
#include <regex>

namespace Modern {

// Modern string functions using C++11 features
namespace StringFunctions {

// Basic string operations
auto to_upper = [](const std::string& str) -> std::string {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
};

auto to_lower = [](const std::string& str) -> std::string {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
};

auto reverse_string = [](const std::string& str) -> std::string {
    std::string result = str;
    std::reverse(result.begin(), result.end());
    return result;
};

auto string_length = [](const std::string& str) -> size_t {
    return str.length();
};

// Advanced string operations
auto trim_whitespace = [](const std::string& str) -> std::string {
    auto start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) return "";
    auto end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
};

auto split_string = [](const std::string& str, char delimiter) -> std::vector<std::string> {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
};

auto join_strings = [](const std::vector<std::string>& strings, const std::string& delimiter) -> std::string {
    if (strings.empty()) return "";
    std::ostringstream oss;
    oss << strings[0];
    for (size_t i = 1; i < strings.size(); ++i) {
        oss << delimiter << strings[i];
    }
    return oss.str();
};

auto count_words = [](const std::string& str) -> size_t {
    std::istringstream iss(str);
    return std::distance(std::istream_iterator<std::string>(iss), 
                        std::istream_iterator<std::string>());
};

auto replace_all = [](std::string str, const std::string& from, const std::string& to) -> std::string {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
};

// Regex-based operations
auto is_email = [](const std::string& str) -> bool {
    std::regex email_pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(str, email_pattern);
};

auto extract_numbers = [](const std::string& str) -> std::vector<int> {
    std::vector<int> numbers;
    std::regex number_pattern(R"(-?\d+)");
    std::sregex_iterator iter(str.begin(), str.end(), number_pattern);
    std::sregex_iterator end;
    
    for (; iter != end; ++iter) {
        numbers.push_back(std::stoi(iter->str()));
    }
    return numbers;
};

} // namespace StringFunctions

// Registration class
class ModernStringRegistrar {
public:
    ModernStringRegistrar() {
        // Register the string group
        MODERN_REGISTER_GROUP(FunctionCategory::STRING, "Modern String Functions");
        
        auto* registry = &ModernRegistry::getInstance();
        
        // Register basic operations
        registry->registerLambda<std::string(const std::string&)>(FunctionCategory::STRING, "to_upper", StringFunctions::to_upper);
        registry->registerLambda<std::string(const std::string&)>(FunctionCategory::STRING, "to_lower", StringFunctions::to_lower);
        registry->registerLambda<std::string(const std::string&)>(FunctionCategory::STRING, "reverse", StringFunctions::reverse_string);
        registry->registerLambda<size_t(const std::string&)>(FunctionCategory::STRING, "length", StringFunctions::string_length);
        
        // Register advanced operations
        registry->registerLambda<std::string(const std::string&)>(FunctionCategory::STRING, "trim", StringFunctions::trim_whitespace);
        registry->registerLambda<std::vector<std::string>(const std::string&, char)>(FunctionCategory::STRING, "split", StringFunctions::split_string);
        registry->registerLambda<std::string(const std::vector<std::string>&, const std::string&)>(FunctionCategory::STRING, "join", StringFunctions::join_strings);
        registry->registerLambda<size_t(const std::string&)>(FunctionCategory::STRING, "count_words", StringFunctions::count_words);
        registry->registerLambda<std::string(std::string, const std::string&, const std::string&)>(FunctionCategory::STRING, "replace_all", StringFunctions::replace_all);
        
        // Register regex operations
        registry->registerLambda<bool(const std::string&)>(FunctionCategory::STRING, "is_email", StringFunctions::is_email);
        registry->registerLambda<std::vector<int>(const std::string&)>(FunctionCategory::STRING, "extract_numbers", StringFunctions::extract_numbers);
    }
};

// Static instance for auto-registration
static ModernStringRegistrar stringRegistrar;

// Explicit initialization function for static linking
void initStringFunctions() {
    static bool initialized = false;
    if (!initialized) {
        // Force the static registrar to be instantiated
        static ModernStringRegistrar registrar;
        initialized = true;
    }
}

} // namespace Modern
