#!/bin/bash

# Build script for Function Register Framework

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Default values
BUILD_TYPE="Release"
BUILD_EXAMPLES="ON"
CLEAN_BUILD="false"
INSTALL="false"

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--debug)
            BUILD_TYPE="Debug"
            shift
            ;;
        -r|--release)
            BUILD_TYPE="Release"
            shift
            ;;
        --no-examples)
            BUILD_EXAMPLES="OFF"
            shift
            ;;
        -c|--clean)
            CLEAN_BUILD="true"
            shift
            ;;
        -i|--install)
            INSTALL="true"
            shift
            ;;
        -h|--help)
            echo "Usage: $0 [OPTIONS]"
            echo "Options:"
            echo "  -d, --debug       Build in debug mode"
            echo "  -r, --release     Build in release mode (default)"
            echo "  --no-examples     Don't build examples"
            echo "  -c, --clean       Clean build directory before building"
            echo "  -i, --install     Install after building"
            echo "  -h, --help        Show this help message"
            exit 0
            ;;
        *)
            print_error "Unknown option: $1"
            echo "Use -h or --help for usage information"
            exit 1
            ;;
    esac
done

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"
BUILD_DIR="$PROJECT_ROOT/build"

print_status "Function Register Framework Build Script"
print_status "Build type: $BUILD_TYPE"
print_status "Build examples: $BUILD_EXAMPLES"
print_status "Project root: $PROJECT_ROOT"
print_status "Build directory: $BUILD_DIR"

# Clean build directory if requested
if [ "$CLEAN_BUILD" = "true" ]; then
    print_status "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
fi

# Create build directory
if [ ! -d "$BUILD_DIR" ]; then
    print_status "Creating build directory..."
    mkdir -p "$BUILD_DIR"
fi

# Change to build directory
cd "$BUILD_DIR" || {
    print_error "Failed to change to build directory"
    exit 1
}

# Configure with CMake
print_status "Configuring with CMake..."
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" \
      -DBUILD_EXAMPLES="$BUILD_EXAMPLES" \
      "$PROJECT_ROOT"

if [ $? -ne 0 ]; then
    print_error "CMake configuration failed"
    exit 1
fi

print_success "CMake configuration completed"

# Build the project
print_status "Building project..."
cmake --build . --config "$BUILD_TYPE" --parallel $(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

if [ $? -ne 0 ]; then
    print_error "Build failed"
    exit 1
fi

print_success "Build completed successfully"

# Install if requested
if [ "$INSTALL" = "true" ]; then
    print_status "Installing..."
    cmake --install . --config "$BUILD_TYPE"
    
    if [ $? -eq 0 ]; then
        print_success "Installation completed"
    else
        print_error "Installation failed"
        exit 1
    fi
fi

# Show build artifacts
print_status "Build artifacts:"
if [ "$BUILD_EXAMPLES" = "ON" ]; then
    if [ -f "examples/func_register_demo" ]; then
        echo "  - examples/func_register_demo"
    fi
    if [ -f "examples/simple_example" ]; then
        echo "  - examples/simple_example"
    fi
fi

if [ -f "src/libfunction_register.a" ]; then
    echo "  - src/libfunction_register.a"
fi

# Provide usage instructions
print_status "Usage:"
if [ "$BUILD_EXAMPLES" = "ON" ]; then
    echo "  Run demo: ./examples/func_register_demo"
    echo "  Run simple example: ./examples/simple_example"
fi
echo "  Link library: -L$BUILD_DIR/src -lfunction_register"
echo "  Include headers: -I$PROJECT_ROOT/include"

print_success "Build script completed successfully!"
