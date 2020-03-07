// Copyright 2020 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "gfx.hpp"

namespace {

bool run = true;
void render(shared_window window) noexcept;

void signal_handler(int signal) {
    run = false;
}

}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "usage: demo <path-to-model>" << std::endl;
        return EXIT_FAILURE;
    }

    std::signal(SIGINT, signal_handler);
    std::signal(SIGHUP, signal_handler);

    try {
        auto window = create_fullscreen_window();
        auto context = create_GL_context(window);
        load_model(argv[1]);
        while (run) {
            render(window);
            // TODO(bkuolt): merge event handling branch here
        }
    } catch (const std::exception &exception) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", exception.what(), nullptr);
        std:: cout << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

namespace {

void render(shared_window window) noexcept {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window.get());
}

}  // namespace
