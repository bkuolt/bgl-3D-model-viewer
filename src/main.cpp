// Copyright 2021 Bastian Kuolt
#include <csignal>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include "gfx/gfx.hpp"
#include "gfx/camera.hpp"

#include <QApplication>
#include <QOpenGLContext>

using namespace bgl;

namespace {




// forward declarations
void create_camera_motion(bool pressed, Camera::horizontal_direction direction, double angle);
void set_up_scene(const std::filesystem::path &path);


}  //


struct {
    SharedMesh mesh;
    SharedGrid grid;
    Camera camera;
    bgl::Camera::shared_motion motion;
} Scene;



void on_render(float delta)  {
    std::cout << "on_render()" << std::endl;
    static bool f = false;
    if (!f) {
        set_up_scene("./assets/models/housemedieval.obj");
        f = true;
    }
   // return;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




    if (Scene.motion) {
        Scene.motion->update();
    }

    const mat4 PV { Scene.camera.getMatrix() };
    Scene.grid->render(PV);
  
  
    Scene.mesh->render(PV);

}

namespace {










class SimpleWindow final : public bgl::Window {
 public:
    SimpleWindow(const std::string &title)
        : bgl::Window( title, on_render )
    {}

 protected:
 #if 0
    void on_key(const SDL_KeyboardEvent &event) override {
        switch (event.keysym.scancode) {
            case SDL_SCANCODE_ESCAPE:
                close();
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
#endif  //0

};

void signal_handler(int signal) {
    std::cerr << console_color::red << "\rrequested program termination" << std::endl;
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

        QApplication app (argc, argv);

    try {
        SimpleWindow window { "BGL Model Viewer" };

        window.show();
        //  set_up_scene(argv[1]);

            
        return app.exec();
       // return window.exec();
    } catch (const std::exception &exception) {
     //   SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", exception.what(), nullptr);
        std::cout << console_color::red << "error: " << exception.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}


/* ------------------------- Details --------------------------- */

namespace {

void create_camera_motion(bool pressed, Camera::horizontal_direction direction, double angle) {
    if (pressed) {
        if (Scene.motion == nullptr) {
            Scene.motion = Scene.camera.createMotion(direction, 85);
        }
    } else {
        if (Scene.motion) {
            Scene.motion->stop();
            Scene.motion.reset();
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

    // on_render(0.0);
}

}  // anonymous namespace
