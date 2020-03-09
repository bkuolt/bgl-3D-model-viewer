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
} App;

namespace {

void load(const std::filesystem::path &path);  // forward declaration

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
        load(argv[1]);
        SDL_ShowWindow(App.window.get());
        loop();
    } catch (const std::exception &exception) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", exception.what(), nullptr);
        std::cout << console_color::red << "error: " << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* ----------------------- Rendering ----------------------- */

namespace {

struct {
    SharedModel model;
    mat4 P;
} Scene;

void load(const std::filesystem::path &path) {
    Scene.model = LoadModel(path);
    // TODO(bkuolt): invegistigate "auto game_controller = get_game_controller();""

    // set up camera
    Scene.P = glm::ortho(-1.0f, 1.0f,
                         -1.0f, 1.0f,
                         10.0f, -10.0f);

    // initialize OpenGL
    SDL_GL_SetSwapInterval(0);  // disable vsync

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);   // TODO(bkuolt): revalidate
}

}  // namespace

void on_render(const SharedWindow &window, float delta) noexcept {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // calculate new camera position
    constexpr float rotation_speed = 30.0f;  // [°/s]
    static float angle = glm::radians(180.0f);
    angle += delta * glm::radians(rotation_speed);

    // update camera position
    const vec3 position { glm::cos(angle), 0.0f, sin(angle) };
    const mat4 MV = glm::lookAt(position, vec3{}, vec3 { 0.0f, 1.0f, 0.0f });

    // render model
    const mat4 MVP = Scene.P * MV;
    RenderModel(Scene.model, MVP);

    SDL_GL_SwapWindow(window.get());
}

/* --------------------- Input Handling -------------------- */

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
