# Basic Software Renderer (SDL3 + C++)

A minimal CPU-based renderer built from scratch using **SDL3** and modern **C++ (C++23/26)**.

This project renders pixels manually into a framebuffer and uploads them to an SDL texture each frame.  
The goal is full control over the math and rendering pipeline.

---

## Features

- Manual framebuffer (`std::vector<Pixel>`)
- Ray generation per pixel
- Custom 3D vector math
- Perspective camera with orthonormal basis
- Rodrigues rotation formula
- Local ↔ Global space transformations
- SDL3 streaming texture pipeline

---

## Rendering Pipeline

Ray generation → Intersection math → Framebuffer → SDL_Texture → Screen

Each frame:

1. Generate ray per pixel
2. Compute direction in camera space
3. Transform to world space
4. Write pixel to framebuffer
5. Lock SDL texture
6. Copy framebuffer memory
7. Unlock and present

---

## Build Instructions (Windows / MSYS2 / MinGW)

Compile with:

```bash
g++ src/main.cpp src/renderer.cpp -o main.exe \
-I "~\code_projects\SDL\x86_64-w64-mingw32\include" \
-L "~\code_projects\SDL\x86_64-w64-mingw32\lib" \
-lSDL3
```

or

```bash
g++ src/main.cpp src/objects.cpp -o main.exe \
-I "~\code_projects\SDL\x86_64-w64-mingw32\include" \
-L "~\code_projects\SDL\x86_64-w64-mingw32\lib" \
-lSDL3
```

Make sure:
- SDL3 DLL is accessible
- Compiler matches SDL build (MinGW if using MinGW SDL)

---

# Mathematics Used

## 1. Ray Equation

Parametric ray:

r(t) = a + t b

Where:
- a = ray origin
- b = direction
- t = distance parameter

---

## 2. Sphere Equation

x² + y² + z² = r²

Substituting ray equation gives a quadratic:

(b·b)t² + 2(a·b)t + (a·a − r²) = 0

---

## 3. Quadratic Formula

t = [−(a·b) ± sqrt((a·b)² − (b·b)((a·a) − r²))] / (b·b)

Intersection exists if:

Δ ≥ 0

---

## 4. Orthonormal Basis

Camera basis vectors:
- i (right)
- j (up)
- k (forward)

Properties:
- i·j = 0
- i·k = 0
- j·k = 0
- |i| = |j| = |k| = 1

Re-orthonormalization ensures numerical stability.

---

## 5. Rodrigues Rotation Formula

v' = v cosθ + (u × v) sinθ + u(u·v)(1 − cosθ)

Where:
- u = axis of rotation
- v = vector being rotated

---

# Camera Model

Perspective projection per pixel:

u = normalized screen x  
v = normalized screen y  
direction = normalize(localToGlobal(u, v, focalLength))

Aspect ratio correction applied.

---

# Core Structures

## Framebuffer

```cpp
std::vector<Pixel> pixels;

Pixel& at(int x, int y) {
    return pixels[y * width + x];
}
```

Row-major memory layout:

index = y * width + x

---

## Perspective Camera

- Position
- Focal length
- Local basis vectors (i, j, k)
- Rotation methods
- Space transformation methods

---

# Design Philosophy

- No external math libraries
- Full manual control
- CPU-only rendering
- Math-first implementation
- Educational clarity over abstraction

---

# Project Goals

- Ray-sphere intersections
- Camera rotation
- Local ↔ world transforms
- Expand toward full raytracer
- Eventually add:
  - Lighting
  - Materials
  - Reflections
  - Shadows

---

# Requirements

- C++23 / C++26 capable compiler
- SDL3 (MinGW build recommended if using MinGW toolchain)
- Windows / MSYS2 environment

---

# Notes

If it works — don’t touch it.

This project is meant to evolve through controlled experimentation and forks.
