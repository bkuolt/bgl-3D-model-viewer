// Copyright 2020 Bastian Kuolt
#include "../App.hpp"
#include "camera.hpp"

#include <stdexcept>
#include <iostream>

namespace bgl {

namespace {

double calculate_aspect_ratio() {
#ifdef DEBUG
    return 16.0 / 9.0;
#else
    const uvec2 size {  () ?   App.window->getSize() : throw std::runtime_error { "could not get aspect ratio" }};
    return size.y ? static_cast<double>(size.x) / size.y : throw std::runtime_error { "invalid aspect ratio" };
#endif  // DEBUG
}

}  // namespace

Camera::Camera() noexcept {
    updateProjectionMatrix();
}

Camera::Camera(const vec3 &position, const vec3 &center)
    : _position { position }, _center { center } {
    updateProjectionMatrix();
    updateViewMatrix();
}

void Camera::updateProjectionMatrix() {
    const double ratio = calculate_aspect_ratio();
    const double width { (ratio / 2) * _zoom };
    _P = glm::frustum(-width, width,
                      -_zoom,  _zoom,
                      1.0, 10.0);
}

void Camera::updateViewMatrix() noexcept {
    _V = glm::lookAt(_position, _center, _up);
}

void Camera::rotate(const vec2 angle) noexcept {
    vec3 position =  glm::rotate(getPosition(), glm::radians(angle.y), _up);
    setPosition(position);
    updateViewMatrix();
}

mat4 Camera::getMatrix() const noexcept {
    return _P * _V;
}

void Camera::setPosition(const vec3 &position) noexcept {
    _position = position;
    updateViewMatrix();
}

void Camera::setZoom(double factor) {
    _zoom = (factor > 0.0) ? factor : throw std::invalid_argument { "invalid zoom factor" };
    updateProjectionMatrix();
}

void Camera::setViewCenter(const vec3 &center) noexcept {
    _center = center;
    updateViewMatrix();
}

const vec3& Camera::getPosition() const noexcept {
    return _position;
}

const vec3& Camera::getViewCenter() const noexcept {
    return _center;
}

double Camera::getZoom() const noexcept {
    return _zoom;
}

/* ----------------------------- Camera Motion ----------------------------- */
camera_motion::camera_motion(Camera &camera, const vec3 &axis, double speed)
    : _camera { camera }, _axis { axis }, _speed { speed }
{}

void camera_motion::start() noexcept {
    _timer.start();
}

void camera_motion::stop() noexcept {
    _timer.stop();
}

bool camera_motion::is_running() const noexcept {
    return !_timer.is_stopped();
}

void camera_motion::update() noexcept {
    if (is_running()) {
        const std::chrono::nanoseconds ns { _timer.elapsed().user };
        const auto elapsed { std::chrono::duration_cast<std::chrono::milliseconds>(ns).count() };
        const float angle { std::fmod(glm::radians(_speed * elapsed), M_2_PI) };

        const vec3 position { glm::rotate(_camera.getPosition(), angle, _camera._up) };
        _camera.setPosition(position);
    }
}

#if 1
Camera::shared_motion Camera::createMotion(horizontal_direction direction, float speed) {
    const auto motion_speed { (direction == horizontal_direction::left) ? speed : -speed };
    return std::make_shared<camera_motion>(*this, _up, motion_speed);
}
#endif  // 1

}  // namespace bgl
