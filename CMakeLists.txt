cmake_minimum_required(VERSION 3.20)
project(bgl-3d-model-viewer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Force-include gfx/gl.hpp first in every translation unit to satisfy GLEW before any Qt/system GL headers
add_compile_options(-include ${CMAKE_CURRENT_SOURCE_DIR}/src/gfx/gl.hpp)

# Find Qt 6
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLEW
find_package(GLEW REQUIRED)

# Find Assimp
find_package(assimp REQUIRED)

# Find GLM
find_package(glm REQUIRED)

# GFX Subsystem
add_library(bgl_gfx STATIC
    src/gfx/bounding_box.cpp
    src/gfx/bounding_box.hpp
    src/gfx/box.cpp
    src/gfx/box.hpp
    src/gfx/camera.cpp
    src/gfx/camera.hpp
    src/gfx/gfx.cpp
    src/gfx/gfx.hpp
    src/gfx/gl.hpp
    src/gfx/grid.cpp
    src/gfx/grid.hpp
    src/gfx/importer.cpp
    src/gfx/importer.hpp
    src/gfx/material.hpp
    src/gfx/math.hpp
    src/gfx/mesh.cpp
    src/gfx/mesh.hpp
    src/gfx/model.cpp
    src/gfx/model.hpp
    src/gfx/scene.hpp
)

target_include_directories(bgl_gfx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(bgl_gfx PUBLIC
    GLEW::GLEW
    OpenGL::GL
    OpenGL::GLU
    Qt6::Core
    Qt6::Gui
    Qt6::OpenGLWidgets
    assimp::assimp
    glm::glm
)

# GUI Subsystem
add_library(bgl_gui STATIC
    src/gui/gui.hpp
    src/gui/menu.cpp
    src/gui/menu.hpp
    src/gui/panel.cpp
    src/gui/panel.hpp
    src/gui/viewport.cpp
    src/gui/viewport.hpp
    src/gui/window.cpp
    src/gui/window.hpp
)

target_include_directories(bgl_gui PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(bgl_gui PUBLIC
    GLEW::GLEW
    bgl_gfx
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
)

# Application Executable Target
add_executable(bgl_demo
    src/main.cpp
    src/window.cpp
    src/window.hpp
)

target_include_directories(bgl_demo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

target_link_libraries(bgl_demo PRIVATE
    bgl_gui
    bgl_gfx
    GLEW::GLEW
)

# Copy shaders and assets to build output directory
add_custom_command(
    TARGET bgl_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_CURRENT_SOURCE_DIR}/src/assets
            $<TARGET_FILE_DIR:bgl_demo>/assets
)
