# Minus-External-Base

A C++ external overlay application built with ImGui and DirectX 11 for Windows.

## Overview

Minus-External is a standalone Windows application that provides an overlay interface with ESP (Extra Sensory Perception) capabilities. The project is built using modern C++ with ImGui for the user interface and DirectX 11 for rendering.

## Features

- **ESP System**: Advanced ESP functionality with customizable features
- **Overlay Interface**: DirectX 11-based overlay with ImGui integration
- **Memory Management**: Robust memory reading and manipulation utilities
- **Mathematical Operations**: Optimized math utilities for 3D calculations
- **Modular Architecture**: Clean separation of concerns with organized feature modules

## Prerequisites

- Visual Studio 2019 or later (with C++ development tools)
- Windows 10/11
- DirectX 11 compatible graphics card

## Build Instructions

### Using Visual Studio

1. Clone the repository:
   ```bash
   git clone <your-repository-url>
   cd Minus-External
   ```

2. Open `Minus-External.vcxproj` in Visual Studio

3. Select your desired configuration:
   - **Debug**: For development and debugging
   - **Release**: For optimized production builds

4. Build the solution:
   - Press `Ctrl+Shift+B` or go to `Build > Build Solution`

### Build Configurations

- **Debug**: Includes debug symbols and logging for development
- **Release**: Optimized build with minimal overhead

## Project Structure

```
Minus-External/
├── features/           # Core feature implementations
│   ├── esp.cpp        # ESP functionality
│   └── esp.h          # ESP header definitions
├── imgui/             # ImGui library files
│   ├── imgui.cpp      # Core ImGui implementation
│   ├── imgui.h        # Main ImGui header
│   ├── imgui_impl_dx11.*  # DirectX 11 implementation
│   └── imgui_impl_win32.* # Win32 platform implementation
├── overlay/           # Overlay system
│   ├── overlay.cpp    # Overlay implementation
│   └── overlay.h      # Overlay header
├── sdk/               # Software Development Kit
│   ├── math.cpp       # Mathematical utilities
│   ├── memory.cpp     # Memory management
│   ├── memory.h       # Memory operation headers
│   ├── offsets.h      # Memory offset definitions
│   └── sdk.h          # Main SDK header
├── main.cpp           # Application entry point
└── x64/               # Build output directory
```

## Usage

1. Run the compiled executable from the Release or Debug folder
2. The overlay will initialize and display the ImGui interface
3. Configure ESP settings through the user interface
4. Use the provided controls to toggle features on/off

## Development

### Adding New Features

1. Create new source files in the `features/` directory
2. Include appropriate headers in your implementation
3. Register new features in the main application loop
4. Update the UI in the overlay system if needed

### Memory Management

The project includes a robust memory management system in the `sdk/` directory:

- `memory.cpp/h`: Core memory operations
- `offsets.h`: Memory offset definitions
- `math.cpp`: Mathematical utilities for calculations

### ImGui Integration

The project uses ImGui for the user interface:

- DirectX 11 rendering backend
- Win32 platform integration
- Custom styling and theming support

## Dependencies

- **ImGui**: Included in the project
- **DirectX 11**: Windows system dependency
- **Windows API**: For platform-specific operations

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## Notes

- Ensure you have proper permissions for memory operations
- Run as administrator if required for certain features
- The application is designed for educational and development purposes

## License

Please ensure you comply with all applicable laws and terms of service when using this software.

## Troubleshooting

### Common Issues

1. **Build Errors**: Ensure you have the latest Visual Studio C++ build tools
2. **DirectX Issues**: Verify DirectX 11 is properly installed
3. **Memory Access**: Run as administrator if experiencing permission issues

### Debug Mode

For debugging, use the Debug configuration which includes:
- Detailed logging
- Debug symbols
- Runtime checks

## Support

For issues and questions, please refer to the project documentation or create an issue in the repository. 
