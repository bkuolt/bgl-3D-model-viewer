// Copyright 2020 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>  // glm::lookAt(), glm::ortho()

#include "gfx.hpp"
#include "input.hpp"

struct App {
    bool run = true;
    SharedWindow window;
    SharedContext context;
    SharedModel model;
} App;

namespace {

void signal_handler(int signal) {
    App.run = false;
    std::cout << console_color::red << "\rrequested program termination" << std::flush;
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
        App.model = LoadModel(argv[1]);

        auto game_controller = get_game_controller();
        SDL_ShowWindow(App.window.get());
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

//glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const vec3 position { 0.0f, 0.0f, -6.0f };
    const mat4 P = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 300.0f, -300.0f);
    const mat4 MV = glm::lookAt(position, vec3{}, vec3 { 0.0f, 1.0f, 0.0f });
    const mat4 MVP = P * MV;
    RenderModel(App.model, MVP);

    SDL_GL_SwapWindow(window.get());
}
