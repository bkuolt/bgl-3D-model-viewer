// Copyright 2020 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <map>
#include <stdexcept>

#include "gfx/gfx.hpp"
#include "input.hpp"
#include "App.hpp"

using namespace bgl;

struct App App;  // declared in App.hpp

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
    Camera camera;
} Scene;

/* --------------------- Input Handling -------------------- */

void update_position(double delta) {
    const Uint8 *keyboard_state = SDL_GetKeyboardState(NULL);
    if (keyboard_state == nullptr) {
        throw std::runtime_error { "could not get SDL keyboard state" };
    }

    const double angle { 10.0 * delta };
    if (keyboard_state[SDL_SCANCODE_LEFT]) {
        Scene.camera.rotate({ angle, 0.0 });
    } else if (keyboard_state[SDL_SCANCODE_RIGHT]) {
        Scene.camera.rotate({ -angle, 0.0 });
    } else if (keyboard_state[SDL_SCANCODE_UP]) {
         Scene.camera.rotate({ 0.0, angle });
    } else if (keyboard_state[SDL_SCANCODE_DOWN]) {
        Scene.camera.rotate({ 0.0, -angle });
    }
}

}  // namespace

void on_key(const SDL_KeyboardEvent &event) {
    switch (event.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
            App.run = false;
            break;
        default:
            // nothing to do
            break;
    }
}

void on_button(ps4_button, bool pressed) {
    std::cout << "game controller button " << (pressed ? "pressed" : "released") << std::endl;
    // TODO(bkuolt)
}

void on_motion(const vec2 &lhs, const vec2 &rhs) {
    std::cout << "dual stick motion [" << lhs << " , " << rhs << "]" << std::endl;
    // TODO(bkuolt): use left hand side as camera rotation controll
    // TODO(bkuolt): use right hand side as zoom level controll
}

void on_trigger(float lhs, float rhs) {
    std::cout << "trigger [" << lhs << " , " << rhs << "] pressed" << std::endl;
    // TODO(bkuolt)
}

/* ------------------------ Rendering ---------------------- */

namespace {

void set_up_scene(const std::filesystem::path &path) {
    if (SDL_GL_SetSwapInterval(0) == -1) {
        throw std::runtime_error { "could not disable vsync" };
    }

    Scene.mesh = LoadMesh(path);
    Scene.camera.setViewCenter({ 0.0, 0.0, 0.0 });
    Scene.camera.setPosition({ 0.0, 0.0, 2.0 });

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

}  // namespace

void on_render(const SharedWindow &window, float delta) noexcept {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    update_position(delta);
    const mat4 VP = Scene.camera.getMatrix();
    Scene.mesh->render(VP);
    SDL_GL_SwapWindow(window.get());
}
