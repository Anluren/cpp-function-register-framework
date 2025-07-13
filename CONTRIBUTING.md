# Contributing to Function Register Framework

Thank you for your interest in contributing to the Function Register Framework! This document provides guidelines and information for contributors.

## Getting Started

### Prerequisites

- C++11 compatible compiler
- CMake 3.10 or later
- Git

### Setting Up Development Environment

```bash
# Clone the repository
git clone <repository-url>
cd func_register

# Build the project
./build.sh --debug

# Run tests
cd build && ./examples/func_register_demo && ./examples/simple_example
```

## Development Guidelines

### Code Style

- Follow C++11 standards and best practices
- Use 4 spaces for indentation (no tabs)
- Maximum line length: 100 characters
- Use descriptive variable and function names
- Add documentation comments for public APIs

### Naming Conventions

- **Classes**: PascalCase (`FunctionGroup`, `ConcreteFunctionGroup`)
- **Functions**: camelCase (`getFunctionNames`, `registerFunction`)
- **Variables**: camelCase (`groupName`, `functionPtr`)
- **Constants**: UPPER_SNAKE_CASE (`MATH_FUNCTIONS`)
- **File names**: snake_case (`function_group.h`, `math_functions.cpp`)

### File Organization

```
func_register/
├── include/          # Public headers only
├── src/             # Core implementation
├── examples/        # Example code and demos
├── docs/           # Documentation
└── tests/          # Unit tests (future)
```

### Header Guidelines

- All public headers go in `include/`
- Use include guards (`#pragma once`)
- Minimize dependencies in headers
- Document all public APIs

### Source Guidelines

- Implementation files go in `src/` or `examples/`
- Keep implementations focused and modular
- Use static initialization for automatic registration
- Handle errors gracefully

## Adding New Features

### Adding Function Group Types

1. Add enum value to `FunctionGroupType` in `include/function_group.h`
2. Create implementation file with auto-registration
3. Add example usage if applicable
4. Update documentation

### Adding New Function Signatures

1. Add function pointer typedef in `include/function_group.h`
2. Add storage map in `ConcreteFunctionGroup`
3. Update registration and calling methods
4. Add example usage
5. Update documentation

### Example: Adding 3-Parameter Functions

```cpp
// 1. Add typedef
typedef void* (*FuncPtr3)(void*, void*, void*);

// 2. Add storage in ConcreteFunctionGroup
std::map<std::string, FuncPtr3> functions3;

// 3. Add registration method
void registerFunction3(const std::string& name, FuncPtr3 func);

// 4. Update callFunction to handle 3 parameters
```

## Submitting Changes

### Pull Request Process

1. **Fork** the repository
2. **Create** a feature branch from `main`
3. **Make** your changes following the guidelines
4. **Test** your changes thoroughly
5. **Commit** with clear, descriptive messages
6. **Push** to your fork
7. **Submit** a pull request

### Commit Message Format

```
type(scope): short description

Longer explanation if needed.

- List specific changes
- Reference issues if applicable

Closes #123
```

Types: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

### Example Commits

```bash
feat(registry): add support for 3-parameter functions

- Add FuncPtr3 typedef and storage map
- Implement registerFunction3 method
- Update callFunction to handle 3 parameters
- Add example usage in math_functions.cpp

Closes #45

fix(build): resolve linking issue with static initializers

- Change CMake linking order for static libraries
- Ensure example functions are properly linked
- Update build script with better error handling

docs(readme): update installation instructions

- Add CMake version requirement
- Clarify build script usage
- Add troubleshooting section
```

## Testing

### Manual Testing

```bash
# Build and test all configurations
./build.sh --debug && cd build
./examples/func_register_demo
./examples/simple_example

./build.sh --release --clean && cd build  
./examples/func_register_demo
./examples/simple_example
```

### Adding Tests

Currently, the project uses example programs for testing. When adding new features:

1. Add usage examples to existing demo programs
2. Create new example programs if needed
3. Document expected behavior
4. Test edge cases and error conditions

Future: We plan to add unit tests using a framework like Google Test.

## Documentation

### Code Documentation

```cpp
/**
 * Brief description of the function
 * 
 * Detailed description if needed.
 * 
 * @param paramName Description of parameter
 * @return Description of return value
 * @throws Description of exceptions (if any)
 * 
 * @example
 * FunctionGroup* group = registry.getGroup(MATH_FUNCTIONS);
 * void* result = group->callFunction("add", &input);
 */
```

### README Updates

When adding features, update relevant documentation:

- Main README.md
- USAGE.md if applicable
- Example code
- Build instructions if needed

## Questions and Support

### Getting Help

- **Issues**: Use GitHub issues for bug reports and feature requests
- **Discussions**: Use GitHub discussions for questions and ideas
- **Email**: Contact maintainers directly for sensitive issues

### Reporting Bugs

Include in your bug report:

1. **Environment**: OS, compiler, CMake version
2. **Steps**: How to reproduce the issue
3. **Expected**: What should happen
4. **Actual**: What actually happens
5. **Code**: Minimal example demonstrating the issue

### Feature Requests

For new features, provide:

1. **Use case**: Why is this feature needed?
2. **Proposal**: How should it work?
3. **Examples**: Show expected usage
4. **Compatibility**: Impact on existing code

## Recognition

Contributors will be acknowledged in:

- README.md contributors section
- Release notes
- Git commit history
- Special recognition for significant contributions

Thank you for contributing to the Function Register Framework!
