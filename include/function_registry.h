#pragma once

// Compatibility header for legacy examples
// This file provides a unified interface for backward compatibility

#include "modern_function_group.h"

// Legacy compatibility aliases
using FunctionGroupType = ::FunctionGroupType;
using FunctionGroup = ModernFunctionGroup;
using FunctionRegistry = ::FunctionRegistry;

// Legacy constants for backward compatibility
constexpr FunctionGroupType MATH_FUNCTIONS = FunctionGroupType::MATH_FUNCTIONS;
constexpr FunctionGroupType STRING_FUNCTIONS = FunctionGroupType::STRING_FUNCTIONS;
constexpr FunctionGroupType UTILITY_FUNCTIONS = FunctionGroupType::UTILITY_FUNCTIONS;
