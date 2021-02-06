// Copyright 2020 Bastian Kuolt
#ifndef APP_HPP_
#define APP_HPP_

#define DEBUG

#include <chrono>  // const std::chrono::milliseconds

#include "gfx/gfx.hpp"  // bgl::SharedWindow, bgl::SharedContext

struct App {
    bool run = true;
    bgl::SharedWindow window;
};

extern struct App App;  // defined in main.cpp

// Input Callbacks
void on_key(const SDL_KeyboardEvent &event);

// Render Callbacks
void on_render(float delta) noexcept;

#endif  // APP_HPP_
