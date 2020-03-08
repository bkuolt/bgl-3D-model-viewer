// Copyright 2020 Bastian Kuolt
#ifndef APP_HPP_
#define APP_HPP_

#include "gfx.hpp"
#include "input.hpp"

extern struct App {
    bool run = true;
    shared_window window;
    shared_context context;
} App;

void on_button(ps4_button, bool pressed);
void on_motion(const vec2 &lhs, const vec2 &rhs);
void render(const shared_window &window);

#endif  // APP_HPP_

