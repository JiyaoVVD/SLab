# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

SLab is a CPU-based software renderer written in C++17 for learning purposes. It renders geometry entirely on the CPU into a raw framebuffer, then blits the result to a GLFW window using OpenGL 2.1 immediate mode (`glDrawPixels`). No GPU shaders are involved in the rendering logic.

## Build

The project uses CMake targeting Visual Studio 2022 on Windows.

**Configure:**
```
cmake -B cmake-build-debug-visual-studio -G "Visual Studio 17 2022"
```

**Build:**
```
cmake --build cmake-build-debug-visual-studio --config Debug
```

Alternatively, open `cmake-build-debug-visual-studio/slab.sln` in Visual Studio directly. The pre-built output is an `slab.exe` in `cmake-build-debug-visual-studio/Debug/`.

There are no tests in this project.

## Architecture

### Type System (`include/stype.h`)

All math primitives are aliased through `S`-prefixed typedefs backed by GLM types. Float precision is controlled by the `SLAB_PRECISION_DOUBLE` preprocessor define — absent it, everything uses `float`.

- `SFloat`, `SVector2/3/4`, `SMatrix3/4` — geometry types, precision-switchable
- `SColor3/4` — normalized float colors (`[0,1]`)
- `SNormColor3/4` — byte colors (`[0,255]`) stored as `glm::uvec3/4`
- `SVertex` — position (`SVector3`) + normal (`SVector3`) + color (`SColor4`)
- `SFragment` — screen-space fragment: coord (`SVector2Int`), normal, color, depth
- `S_CONST_FLOAT(n)` macro — appends `f` suffix when in single-precision mode
- `NORMALIZE_COLOR_3/4` macros — convert float colors to byte colors with clamping
- `SConst` namespace — `up`, `right`, `forward` unit vectors; `identity3/4` matrices

### Rendering Pipeline

The pipeline is entirely software-driven:

1. **Vertex input** — `Renderer::setVertices()` uploads geometry into `vertexBuffer` / `indexBuffer`
2. **Vertex processing** — `processVertex()` applies camera view × projection matrices, then viewport transform to screen-pixel coordinates
3. **Rasterization** — `rasterizeTriangle()` iterates a bounding box, tests point-in-triangle with integer 2D cross products (`icross2`), interpolates depth and color via barycentric weights, and pushes fragments into `fragBuffer`
4. **Fragment output** — fragments in `fragBuffer` are depth-tested and written to the CPU framebuffer with `DrawPixel()`
5. **Display** — `RenderFrameBuffer()` calls `glDrawPixels` to upload the buffer to OpenGL, then `glfwSwapBuffers` to present

Render modes: `SLAB_TRIANGLE_MODE`, `SLAB_LINE_MODE`, `SLAB_LINE_LOOP_MODE`.

### Key Source Files

| File | Role |
|---|---|
| `include/stype.h` | All type aliases, structs (`SVertex`, `SFragment`), constants, color macros |
| `include/math_utils.h` | `icross2`, `PrintMatrix<>`, `PrintVector<>` templates |
| `src/framebuffer.h/cpp` | CPU framebuffer + depth buffer; `DrawPixel`, `DrawLine`, `DrawTriangle`, `RenderFrameBuffer` |
| `src/camera.h/cpp` | View and projection matrices; supports perspective and orthographic |
| `src/renderer.h/cpp` | Full pipeline: vertex processing, rasterization, depth test, frame dispatch |
| `src/main.cpp` | GLFW window creation, camera orbit animation loop |
| `src/Primitive.h` | Placeholder for future primitive geometry helpers (currently empty) |

### Dependencies (vendored)

- **GLFW** — pre-built for VC2022 at `libs/glfw/lib-vc2022/`
- **GLM** — header-only at `libs/glm/`; included directly, not via CMake's `find_package`

Intel TBB (`oneapi/tbb`) is referenced but commented out throughout — it was intended for parallelizing the rasterization loop.
