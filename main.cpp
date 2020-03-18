// Copyright 2020 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <glm/gtc/matrix_transform.hpp>  // glm::lookAt(), glm::ortho()

#include "gfx/gfx.hpp"
#include "input.hpp"
#include "App.hpp"

using namespace bgl;

struct App {
    bool run = true;
    bgl::SharedWindow window;
    bgl::SharedContext context;
    Camera camera;
} App;  // declared in App.hpp

namespace {

void set_up_scene(const std::filesystem::path &path);  // forward declaration

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
        set_up_scene(argv[1]);
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
    SharedMesh mesh;
    mat4 P;
} Scene;

void set_up_scene(const std::filesystem::path &path) {
    Scene.mesh = LoadMesh(path);

    int width, height;
    SDL_GetWindowSize(App.window.get(), &width, &height);
    const double ratio = static_cast<double>(width) / height;
    Scene.P = glm::frustum(-ratio /2, ratio / 2, -1.0, 1.0, 1.0, 10.0);

    // initialize OpenGL
    if (SDL_GL_SetSwapInterval(0) == -1) {  // disable vsync for benchmarking
        std::cout << "SDL_GL_SetSwapInterval() failed" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

#if 0
double update_angle(double delta) {
    constexpr double rotation_speed = 30.0f;  // [°/s]
    static double angle = glm::radians(180.0f);
    angle += delta * rotation_speed;
    return angle;
}
#endif

}  // namespace

void on_render(const SharedWindow &window, float delta) noexcept {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    const mat4 MVP = Scene.P * App.camera.get_matrix();
    Scene.mesh->render(MVP);
    SDL_GL_SwapWindow(window.get());
}

/* --------------------- Input Handling -------------------- */

void handle_event(const SDL_KeyboardEvent &event) {
    constexpr double rotation_delta = 10.0;

    switch (event.keysym.sym) {
        case SDLK_ESCAPE:
            App.run = false;
            break;
        case SDLK_LEFT:
            App.camera.rotate(rotation_delta, 0.0);
        case SDLK_RIGHT:
            App.camera.rotate(-rotation_delta, 0.0);
            break;
        case SDLK_UP:
            App.camera.rotate(rotation_delta, 0.0);
        case SDLK_DOWN:
            App.camera.rotate(-rotation_delta, 0.0);
            break;
    }
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
