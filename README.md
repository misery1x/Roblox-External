[![Language](https://img.shields.io/badge/Language-C++-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-Educational-green.svg)](#license)
[![DirectX](https://img.shields.io/badge/Graphics-DirectX%2011-red.svg)](https://docs.microsoft.com/en-us/windows/win32/direct3d11/atoc-dx-graphics-direct3d-11)

![image](https://github.com/user-attachments/assets/6c157248-7d93-478b-b96b-730f8399f5b2)

## 📋 Table of Contents
- [Features](#features)
- [Technical Implementation](#technical-implementation)
- [Project Structure](#project-structure)
- [Building](#building)
- [Usage](#usage)
- [Educational Concepts](#educational-concepts)
- [Contributing](#contributing)
- [Disclaimer](#disclaimer)

## ✨ Features
### Core Functionality
- 🎨 **Real-time Overlay Rendering** - DirectX 11 + ImGui integration
- 🧮 **3D to 2D Projection** - Complete WorldToScreen implementation with detailed math
- 🔍 **Memory Reading System** - Process attachment and safe memory operations
- ⚡ **Performance Optimized** - Caching system to minimize memory reads
- 🏗️ **Modular Architecture** - Clean separation of concerns

### Technical Features
- **Error Handling** - Robust error checking and graceful degradation
- **Modern C++** - Uses C++17 features and best practices

### WorldToScreen Pipeline
```cpp
// Step 1: Calculate depth from camera
float w = view_matrix * world_position

// Step 2: Perspective division  
float inv_w = 1.0f / w

// Step 3: Transform to normalized coordinates
screen_pos = (view_matrix * world_position) * inv_w

// Step 4: Convert to screen pixels
final_pos = (screen_size * 0.5f) * (screen_pos + 1.0f)
```

### Memory Architecture
- **Process Attachment** - Opens handle with read permissions
- **Offset-based Reading** - Uses static offsets for game data structures
- **Caching Layer** - Stores frequently accessed data with TTL system
- **String Handling** - Custom string reading for game-specific formats

## 📁 Project Structure

```
SimpleESP/
├── 📁 sdk/                    # Core systems
│   ├── memory.h/cpp          # Memory reading interface
│   ├── sdk.h                 # Data structures and types
│   ├── math.cpp              # Vector/matrix mathematics
│   └── offsets.h             # Memory offset definitions
├── 📁 overlay/               # Rendering system
│   ├── overlay.h/cpp         # DirectX 11 + ImGui integration
│   └── ...
├── 📁 features/              # ESP functionality
│   ├── esp.h/cpp             # Main ESP implementation
│   └── ...
└── main.cpp                  # Application entry point
```

## 🔨 Building

### Prerequisites
- **Visual Studio 2019/2022** with C++ development tools
- **Windows SDK 10.0+**
- **DirectX 11 SDK** (included with Windows SDK)

### Build Steps
1. **Clone the repository**
   ```bash
   git clone https://github.com/yourusername/SimpleESP.git
   cd SimpleESP
   ```

2. **Open in Visual Studio**
   ```bash
   # Open the solution file
   start Minus-External.sln
   ```

3. **Configure build settings**
   - Set configuration to `Release`
   - Set platform to `x64`
   - Ensure Windows SDK version matches your system

4. **Build the project**
   - Press `Ctrl+Shift+B` or use Build → Build Solution

## 🚀 Usage

### Basic Usage
1. **Launch the target application** you want to analyze
2. **Run SimpleESP as Administrator** (required for memory reading)
3. **The overlay will appear** showing real-time ESP information
4. **Press Ctrl+C** in console to exit

### Code Integration
```cpp
// Initialize the ESP system
if (!ActorLoop->Initialize()) {
    // Handle initialization failure
    return -1;
}

// Initialize overlay rendering
if (!OverlayInstance->Initialize()) {
    // Handle overlay failure  
    return -1;
}

// Main rendering loop
while (OverlayInstance->IsRunning()) {
    OverlayInstance->BeginFrame();
    ActorLoop->Render(OverlayInstance.get());
    OverlayInstance->EndFrame();
}
```

## 📚 Educational Concepts

This project demonstrates several important concepts:

### 3D Graphics Programming
- **View Matrix Mathematics** - How cameras transform 3D space
- **Projection Theory** - Converting 3D coordinates to 2D screen space
- **Perspective Division** - Handling depth and distance scaling
- **Coordinate Systems** - World space vs. screen space transformations

### Memory Management
- **Process Memory Reading** - Safe access to external process data
- **Pointer Traversal** - Following memory references through game objects
- **Caching Strategies** - Optimizing performance through intelligent data storage
- **Error Handling** - Graceful handling of invalid memory access

### Software Architecture  
- **Modular Design** - Separation of rendering, memory, and logic systems
- **RAII Patterns** - Smart pointer usage for automatic resource management
- **Interface Design** - Clean APIs for complex functionality
- **Performance Optimization** - Minimizing overhead in real-time systems

## 🤝 Contributing

We welcome contributions that improve the educational value of this project!

### How to Contribute
1. **Fork** the repository
2. **Create** a feature branch (`git checkout -b feature/educational-improvement`)
3. **Commit** your changes (`git commit -am 'Add detailed explanation of matrix math'`)
4. **Push** to the branch (`git push origin feature/educational-improvement`)
5. **Open** a Pull Request

### Contribution Guidelines
- **Focus on Education** - Prioritize clear explanations over complex features
- **Document Everything** - Add comments explaining the "why" behind code
- **Performance Matters** - Maintain real-time performance requirements
- **Cross-Platform** - Consider compatibility where possible

## ⚖️ Disclaimer

**Educational Purpose Only**: This project is created for educational purposes to demonstrate programming concepts including 3D graphics, memory management, and software architecture. 

**Responsible Use**: Users are responsible for ensuring their use complies with applicable laws, terms of service, and ethical guidelines. The authors do not condone or support any malicious use of these techniques.

**Learning Resource**: This code serves as a learning tool for understanding game development, computer graphics, and systems programming concepts.

---

<div align="center">

**Built with ❤️ by Stackswise**

[Report Bug](https://github.com//misery1x/Roblox-External/issues) • [Request Feature](https://github.com/misery1x/Roblox-External/issues)

</div> 
