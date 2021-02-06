// Copyright 2020 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "gfx/gfx.hpp"
#include "gfx/camera.hpp"
#include "App.hpp"


using namespace bgl;

namespace {

struct  {
    bgl::SharedWindow window;
} App;

struct {
    SharedMesh mesh;
    SharedGrid grid;
    Camera camera;
} Scene;

bgl::Camera::shared_motion motion;

void set_up_scene(const std::filesystem::path &path);
void create_camera_motion(bool pressed, Camera::horizontal_direction direction, double angle);


void signal_handler(int signal) {
    std::cout << console_color::red << "\rrequested program termination" << std::flush;
    std::exit(EXIT_FAILURE);
}

}  // anomous namespace

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
        App.window = createWindow("BGL Model Viewer");
        set_up_scene(argv[1]);
        App.window->show();
        loop();
    } catch (const std::exception &exception) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", exception.what(), nullptr);
        std::cout << console_color::red << "error: " << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* 
-------------------------------------------------------------
-                        Callbacks                          -
-------------------------------------------------------------*/
void on_key(const SDL_KeyboardEvent &event) {
    switch (event.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
            App.window->close();
            break;

        case SDL_SCANCODE_LEFT:
            create_camera_motion(event.state == SDL_PRESSED, Camera::horizontal_direction::left, 85);
            break;
        case SDL_SCANCODE_RIGHT:
            create_camera_motion(event.state == SDL_PRESSED, Camera::horizontal_direction::right, 85);
            break;
        default:
            // nothing to do yet
            break;
    }
}

void on_render(float delta) noexcept {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (motion) {
        motion->update();
    }

    const mat4 PV { Scene.camera.getMatrix() };
    Scene.grid->render(PV);
    Scene.mesh->render(PV);
}


/* ------------------------- Details --------------------------- */
namespace {

void create_camera_motion(bool pressed, Camera::horizontal_direction direction, double angle) {
    if (pressed) {
        if (motion == nullptr) {
            motion = Scene.camera.createMotion(direction, 85);
        }
    } else {
        if (motion) {
            motion->stop();
            motion.reset();
        }
    }
}

void set_up_scene(const std::filesystem::path &path) {
    Scene.mesh = LoadMesh(path);
    Scene.camera.setViewCenter({ 0.0, 0.0, 0.0 });
    Scene.camera.setPosition({ 0.0, 1.0, 2.0 });

    Scene.grid = CreateGrid(0.125, 40);
    const vec3 v { 0.0, -Scene.mesh->getBoundingBox().getSize().y / 2.0, 0.0 };
    Scene.grid->translate(v);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // enables OpenGL Antialiasing
    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    on_render(0.0);
}

}  // anonymous namespace
