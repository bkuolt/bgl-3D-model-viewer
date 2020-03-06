// Copyright 2020 Bastian Kuolt
#include <SDL2/SDL.h>
#include<SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>

#include "gfx.hpp"

std::shared_ptr<SDL_Window> create_fullscreen_window() {
    SDL_Window * const window = SDL_CreateWindow("BGL Tech Demo",
                                                 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                                 800, 600,
                                                 SDL_WINDOW_OPENGL);
    if (window == nullptr) {
        throw std::runtime_error{ SDL_GetError() };
    }
    const auto Deleter = [] (SDL_Window *window) { SDL_DestroyWindow(window); };
    return std::shared_ptr<SDL_Window>(window, Deleter);
}

std::shared_ptr<SDL_Renderer> create_renderer(const std::shared_ptr<SDL_Window> &window) {
    auto renderer = SDL_CreateRenderer(window.get(),
                                     -1,
                                      0 /* TODO(bkuol): set flags */);
    if (renderer == nullptr) {
        throw std::runtime_error{ SDL_GetError() };
    }
    const auto Deleter = [] (SDL_Renderer *renderer) { SDL_DestroyRenderer(renderer); };
    return std::shared_ptr<SDL_Renderer>(renderer, Deleter);
}