# 🌋 Vulkan Engine

A lightweight Vulkan rendering engine built from scratch for real-time graphics experimentation and learning. This engine serves as a foundation for building more advanced rendering features and systems.

## ✨ Features

- 🔧 Vulkan boilerplate: instance, physical/logical devices, swapchain, synchronization
- 🎨 Texture loading with mipmapping support
- 📦 OBJ model loading
- 🔍 Basic mipmapping system
- 🖼️ MSAA (4x) for improved visual quality
- 🎥 Camera system
- 💡 Basic lighting system (Ambient and Diffuse)
- 🧊 Added Cubemap
- ⛰️ Terrain Rendering Using Perlin Noise:
  
    <img width="724" height="547" alt="image" src="https://github.com/user-attachments/assets/edb4ebad-3344-425a-9540-a5fcab473c8e" />
    <img width="724" height="501" alt="image" src="https://github.com/user-attachments/assets/faf67864-fc53-48f5-a90e-54ddd7a91651" />

   🧩Vertex Shader Highlights

   -  Fractal Brownian Motion (fBm): Generates the base terrain elevation with multiple octaves of Perlin-like noise for smooth, natural variation.
    
   -  Ridged Multifractal Noise: Adds mountain ranges and sharper ridges for more dramatic topography.
    
   -  Layered Terrain Composition: Combines low-frequency terrain shape, mid-frequency hills, and high-frequency ridges for balanced realism.
    
   -  Dynamic Water Level Flattening: Automatically flattens low-lying regions below a defined water threshold, creating seamless lakes and oceans.
    
   -  Procedural Normal Calculation: Computes terrain normals numerically from nearby height samples to enhance lighting response without a normal map.
 
     
  🎨Fragment Shader Highlights
   -  Biome-Based Coloring: Dynamically assigns colors based on terrain height and slope — including deep ocean, shallow water, beaches, grasslands, forests, rocky cliffs, and snowcaps.
     
   -  Height-Sensitive Blending: Uses smooth interpolation for realistic transitions between terrain materials.
     
   -  Procedural Surface Variation: Adds subtle color noise to break repetition and increase natural diversity.
     
   -  Custom Lighting Model: Implements a Lambertian diffuse model with ambient light.
     
   -  Height-Influenced Ambient Lighting: Simulates atmospheric effects by adjusting ambient strength based on elevation.


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
