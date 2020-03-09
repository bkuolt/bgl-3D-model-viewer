// Copyright 2020 Bastian Kuolt
#ifndef APP_HPP_
#define APP_HPP_

#include <chrono>  // const std::chrono::milliseconds

#include "gfx.hpp"
#include "input.hpp"

extern struct App {
    bool run = true;
    SharedWindow window;
    SharedContext context;
} App;  // defined in main.cpp

void on_button(ps4_button, bool pressed);
void on_motion(const vec2 &lhs, const vec2 &rhs);
void on_trigger(const float lhs, float rhs);
void on_render(const SharedWindow &window, float delta) noexcept;

#endif  // APP_HPP_

