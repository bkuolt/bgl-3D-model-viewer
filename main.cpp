// Copyright 2020 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <thread>

#include "gfx.hpp"
#include "input.hpp"

struct App {
    bool run = true;
    shared_window window;
    shared_context context;
} App;

namespace {

void signal_handler(int signal) {
    App.run = false;
}

}  // namespace

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << "usage: demo <path-to-model>" << std::endl;
        return EXIT_FAILURE;
    }

    std::signal(SIGINT, signal_handler);
    std::signal(SIGHUP, signal_handler);

    try {
        App.window = create_fullscreen_window();
        App.context = create_GL_context(App.window);

        load_model(argv[1]);
        auto game_controller = get_game_controller();
        if (game_controller.wait_for(std::chrono::milliseconds()) == std::future_status::ready) {
            game_controller.get();
        }

        loop();
    } catch (const std::exception &exception) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", exception.what(), nullptr);
        std:: cout << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void on_button(ps4_button, bool pressed) {
    std::cout << "game controller button " << (pressed ? "pressed" : "released") << std::endl;
    // TODO(bkuolt): implement
}

void on_motion(const vec2 &lhs, const vec2 &rhs) {
    std::cout.precision(2);

    std::cout << "dual stick motion ["
                << "(" << std::fixed << lhs.x << " | " << std::fixed << lhs.y << ")"
                << " , "
                << "(" << std::fixed << rhs.x << " | " << std::fixed << rhs.y << ")"
                << "]" << std::endl;
    // TODO(bkuolt): implement
}

void render(const shared_window &window) noexcept {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window.get());
}
