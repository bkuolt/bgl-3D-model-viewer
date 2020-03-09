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
    SharedWindow window;
    SharedContext context;
} App;

namespace {

void signal_handler(int signal) {
    App.run = false;
    std::cout << console_color::red << "requested program termination" << std::endl;
}

}  // namespace

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << console_color::red << "usage: "
                  << console_color::white << "./demo "
                  << console_color::blue << "<path-to-model>" << std::endl;
        return EXIT_FAILURE;
    }

    std::signal(SIGINT, signal_handler);
    std::signal(SIGHUP, signal_handler);

    try {
        App.window = createFullScreenWindow();
        App.context = createGLContext(App.window);

        LoadModel(argv[1]);
        auto game_controller = get_game_controller();
        if (game_controller.wait_for(std::chrono::milliseconds()) == std::future_status::ready) {
            game_controller.get();
        }

        loop();
    } catch (const std::exception &exception) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", exception.what(), nullptr);
        std::cout << console_color::red << "error: "
                  << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void on_button(ps4_button, bool pressed) {
    std::cout << "game controller button " << (pressed ? "pressed" : "released") << std::endl;
    // TODO(bkuolt): implement
}

void on_motion(const vec2 &lhs, const vec2 &rhs) {
    std::cout << "dual stick motion [" << lhs << " , " << rhs << "]" << std::endl;
    // TODO(bkuolt): implement game logic
}

void on_trigger(float lhs, float rhs) {
    std::cout << "trigger [" << lhs << " , " << rhs << "]" << std::endl;
    // TODO(bkuolt): implement game logic
}

void on_render(const SharedWindow &window) noexcept {
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SDL_GL_SwapWindow(window.get());
}
