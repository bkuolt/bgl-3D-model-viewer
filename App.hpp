// Copyright 2020 Bastian Kuolt
#ifndef APP_HPP_
#define APP_HPP_

#include <chrono>  // const std::chrono::milliseconds

#include "gfx/gfx.hpp"  // bgl::SharedWindow, bgl::SharedContext
#include "input.hpp"

struct App {
    bool run = true;
    bgl::SharedWindow window;
};

extern struct App App;  // defined in main.cpp

// Input Callbacks
void on_button(bgl::ps4_button, bool pressed);
void on_motion(const bgl::vec2 &lhs, const bgl::vec2 &rhs);
void on_trigger(const float lhs, float rhs);
void on_key(const SDL_KeyboardEvent &event);

// Render Callbacks
void on_render(float delta) noexcept;

#endif  // APP_HPP_
