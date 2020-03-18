// Copyright 2020 Bastian Kuolt
#ifndef APP_HPP_
#define APP_HPP_

#include <chrono>  // const std::chrono::milliseconds

#include "gfx/gfx.hpp"
#include "input.hpp"

class Camera {
 public:
    static constexpr double _radius = 2.0;

    Camera() {
#if 0
        const double angle { glm::radians(update_angle(delta)) };

        Scene.angle = angle;
        const vec3 position { radius * glm::cos(angle), 0.0f, radius * sin(angle) };
        return glm::lookAt(position, { 0.0, 0.0, 0.0}, vec3 { 0.0f, 1.0f, 0.0f });
#endif
    }

    void rotate(double x /* [°] */, double y /* [°] */) {
        // TODO(bkuolt)
    }

    glm::mat4 get_matrix() {
        return {};  // TODO(bkuolt)
    }
};

extern struct App {
    bool run = true;
    bgl::SharedWindow window;
    bgl::SharedContext context;
    Camera camera;
} App;  // defined in main.cpp

void on_button(bgl::ps4_button, bool pressed);
void on_motion(const bgl::vec2 &lhs, const bgl::vec2 &rhs);
void on_trigger(const float lhs, float rhs);
void handle_event(const SDL_KeyboardEvent &event);

void on_render(const bgl::SharedWindow &window, float delta) noexcept;

#endif  // APP_HPP_

