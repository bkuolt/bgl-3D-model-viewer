// Copyright 2020 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "gfx.hpp"

namespace {

bool run = true;

void signal_handler(int signal) {
    run = false;
}

}

int main(int argc, char *argv[]) {
    std::signal(SIGINT, signal_handler);
    std::signal(SIGHUP, signal_handler);

    try {
        auto window = create_fullscreen_window();
        auto renderer = create_renderer(window);
        while (run) {
            SDL_RenderClear(renderer.get());
            SDL_RenderPresent(renderer.get());
        }
    } catch (const std::exception &exception) {
        std:: cout << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

namespace {

void load_assets() {
    // TODO(bkuolt): load model
}

}
