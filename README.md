# 🎯 SimpleESP - Minus External Base

[![Language](https://img.shields.io/badge/Language-C++-blue.svg)](https://isocpp.org/)
[![Platform](https://img.shields.io/badge/Platform-Windows-lightgrey.svg)](https://www.microsoft.com/windows)
[![License](https://img.shields.io/badge/License-Educational-green.svg)](#disclaimer)
[![DirectX](https://img.shields.io/badge/Graphics-DirectX%2011-red.svg)](https://learn.microsoft.com/en-us/windows/win32/direct3d11/)

![Preview](https://github.com/user-attachments/assets/6c157248-7d93-478b-b96b-730f8399f5b2)

## ✅ Features

- 🖼️ Overlay rendering using **DirectX 11 + ImGui**
- 🎯 3D to 2D projection (**WorldToScreen**) with matrix math
- 🧠 Memory reading using process handles and offsets
- 🚀 Optimized with caching and modular design
- ⚙️ Clean architecture using modern C++17

---

## 📁 Folder Structure
SimpleESP/
├── sdk/ // Math, memory, offsets
├── overlay/ // DirectX + ImGui overlay
├── features/ // ESP logic
└── main.cpp // Entry point

---

## ⚒️ How to Build

**Requirements:**
- Visual Studio 2019 or 2022
- Windows SDK 10.0+
- C++ toolset + DirectX (included with Windows SDK)

**Steps:**
1. Clone the repo:
2. git clone https://github.com/yourusername/SimpleESP.git
cd Roblox-External
2. Open `Minus-External.sln` in Visual Studio  
3. Set config to `Release` and platform to `x64`  
4. Build → Build Solution (`Ctrl + Shift + B`)

---

## ▶️ How to Use

1. Launch the game/app you want ESP for  
2. Run `SimpleESP.exe` as **Administrator**  
3. Overlay appears automatically  
4. Press `Ctrl + C` to exit  

---

### Example Code

if (!ActorLoop->Initialize()) return -1;
if (!OverlayInstance->Initialize()) return -1;

while (OverlayInstance->IsRunning())
{
    OverlayInstance->BeginFrame();
    ActorLoop->Render(OverlayInstance.get());
    OverlayInstance->EndFrame();
}

⚠️ Disclaimer
This project is for educational use only.
It demonstrates memory reading, overlays, and 3D graphics in C++.
Please use responsibly and comply with all applicable laws and terms.

Made with ❤️ by Stackswise
🐛 Report Bug • ✨ Request Feature
