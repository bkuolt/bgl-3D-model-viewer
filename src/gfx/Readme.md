# BGL GFX Subsystem

The `bgl::gfx` subsystem provides the core rendering engine abstractions and OpenGL graphics pipeline management for the BGL 3D Model Viewer.

## Components

- `gl.hpp`: Master OpenGL header wrapper bringing in GLEW and core GL function bindings.
- `math.hpp`: Vector, matrix, and mathematical utility types built upon GLM.
- `mesh.hpp` / `mesh.cpp`: Low-level OpenGL buffer abstractions managing VBOs, IBOs, and VAOs.
- `model.hpp` / `model.cpp`: High-level 3D renderable object containing collections of meshes and materials.
- `camera.hpp` / `camera.cpp`: Orbital ArcBall and perspective projection camera matrices.
- `importer.hpp` / `importer.cpp`: Asset loading via Assimp for 3D model formats (OBJ, PLY, FBX) and QOpenGLTexture images.
- `bounding_box.hpp` / `bounding_box.cpp`: Axis-Aligned Bounding Box (AABB) spatial boundary calculation.
- `box.hpp` / `box.cpp`: Wireframe bounding box renderer.
- `grid.hpp` / `grid.cpp`: Dynamic 3D ground reference grid renderer.
- `scene.hpp`: Environment definitions including directional lighting sources.