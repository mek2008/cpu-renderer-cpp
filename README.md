# CPU Ray Tracer

**Version 3.0**

A CPU-based ray tracer written in C++20, featuring custom BVH acceleration, recursive ray tracing and multithreaded rendering.

## Features

- Custom Bounding Volume Hierarchy (BVH).
- Triangle-based geometry.
- BVH-accelerated ray traversal.
- Multithreaded CPU rendering.
- Configurable anti-aliasing.
- Point lighting and shadows.
- Recursive reflections.
- Transparency and alpha compositing.
- Reinhard tone mapping.
- BMP image output.

## Version 3.0

Version 3.0 introduces the current rendering architecture, including BVH acceleration, recursive reflections, transparency and multithreaded rendering.

The renderer is currently focused on triangle-based geometry and custom scene construction.

## Roadmap

### Version 3.1

Planned additions:

- Wavefront `.obj` model loading.
- Phong shading.
- Refraction.

## Technical Details

- Language: C++20
- Rendering: CPU
- Acceleration: Custom BVH
- Output: BMP

## License

See `LICENSE` for licensing information.

## Additional

compiler flags and info for testing: g++ -std=c++20 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wsign-conversion -Wold-style-cast -Wnon-virtual-dtor -Wnull-dereference -Wuseless-cast -g -fsanitize=address,undefined -DDEBUG src/*.cpp -o render

non testing: g++ -std=c++20 -g -fsanitize=address,undefined -DDEBUG src/*.cpp -o render && ASAN_OPTIONS=detect_leaks=0 ./render
comment: add -O2 for optimizer afrer -g

Claude was used to generate scenes.
