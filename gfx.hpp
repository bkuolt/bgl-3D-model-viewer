// Copyright 2020 Bastian Kuolt
#ifndef GFX_HPP_
#define GFX_HPP_

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <memory>

std::shared_ptr<SDL_Window> create_fullscreen_window();
std::shared_ptr<SDL_Renderer> create_renderer(const std::shared_ptr<SDL_Window> &window);

#endif  // GFX_HPP_
