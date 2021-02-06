// Copyright 2020 Bastian Kuolt
#ifndef APP_HPP_
#define APP_HPP_

#define DEBUG

struct SDL_KeyboardEvent;

// Callbacks
void on_key(const SDL_KeyboardEvent &event);
void on_render(float delta) noexcept;

#endif  // APP_HPP_
