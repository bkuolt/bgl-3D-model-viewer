# BGL GUI Subsystem

The `bgl::gui` subsystem implements the user interface components built with Qt 6.

## Components

- `gui.hpp`: Master umbrella header including all GUI subsystem components.
- `window.hpp` / `window.cpp`: Base application window inheriting from `QMainWindow`.
- `viewport.hpp` / `viewport.cpp`: Qt 6 OpenGL widget viewport (`QOpenGLWidget`) managing render loops and frame counters.
- `menu.hpp` / `menu.cpp`: Application menu bar providing file loading dialogs and help menus.
- `panel.hpp` / `panel.cpp`: Side control panels providing rendering options and scene statistics.