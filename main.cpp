// Copyright 2020 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "gfx.hpp"

namespace {

struct {
    bool run = true;
    std::shared_ptr<SDL_Window> window;
    std::shared_ptr<SDL_Renderer> renderer;
} App;

void loop();  // forward declaration

void signal_handler(int signal) {
    App.run = false;
}

}  // namespace

int main(int argc, char *argv[]) {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGHUP, signal_handler);

    try {
        App.window = create_fullscreen_window();
        App.renderer = create_renderer(App.window);
        loop();
    } catch (const std::exception &exception) {
        std:: cout << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

namespace {

void handle_event(const SDL_Event &event) {
    switch (event.type) {
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                App.run = false;
            }
            break;
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            App.run = false;
            break;
    }
}

void loop() {
    SDL_Event event;
    while (App.run) {
        SDL_RenderClear(App.renderer.get());
        SDL_RenderPresent(App.renderer.get());
        while (SDL_PollEvent(&event)) {
            handle_event(event);
        }
    }
}

}  // namespace
