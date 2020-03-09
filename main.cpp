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

        //auto game_controller = get_game_controller();
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

    static Uint32 timestamp = SDL_GetTicks();
    static int fps = 0;

    ++fps;

    auto duration = SDL_GetTicks() - timestamp;
    if (duration >= 1000) {
        timestamp = SDL_GetTicks();

        std::cout << console_color::blue
                  << "\r" << fps << " fps"
                  << console_color::white << std::flush;
        fps = 0;
    }




    static auto last_rendering_timepoint = std::chrono::steady_clock::now();

    const float delta = 
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - last_rendering_timepoint).count() / 1000.0;
    last_rendering_timepoint = std::chrono::steady_clock::now();


    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    const mat4 P = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 300.0f, -300.0f);

    static float angle = 0;
    angle += delta * 10 /* 10° per second */;
    const vec3 position {
        glm::cos(glm::radians(angle)),
        0.0f,
        -glm::sin(glm::radians(180 + angle))
    };

    const mat4 MV = glm::lookAt(position, vec3{}, vec3 { 0.0f, 1.0f, 0.0f });
    const mat4 MVP = P * MV;
    RenderModel(App.model, MVP);

    SDL_GL_SwapWindow(window.get());
    SDL_GL_SetSwapInterval(0 /* no vsync */);
}
