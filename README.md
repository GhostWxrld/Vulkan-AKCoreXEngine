# 🌋 Vulkan Engine

A lightweight Vulkan rendering engine built from scratch for real-time graphics experimentation and learning. This engine serves as a foundation for building more advanced rendering features and systems.

## ✨ Features

- 🔧 Vulkan boilerplate: instance, physical/logical devices, swapchain, synchronization
- 🎨 Texture loading with mipmapping support
- 📦 OBJ model loading
- 🔍 Basic minimap/mipmapping system
- 🖼️ MSAA (4x) for improved visual quality
- 🎥 Camera system
- 💡 Basic lighting system (Ambient and Diffuse)


## 🛠️ Build Requirements

- Vulkan SDK 1.3+
- CMake 3.20+
- C++17-compatible compiler (MSVC, GCC, or Clang)
- GPU with Vulkan support

## 🚀 Build Instructions

```bash
git clone https://github.com/yourusername/vulkan-engine.git
cd vulkan-engine
mkdir build && cd build
cmake ..
cmake --build . --config Release
