// Copyright 2020 Bastian Kuolt
#include "camera.hpp"

#include <stdexcept>
#include <iostream>


namespace bgl {

namespace {

double calculate_aspect_ratio() {
#define DEBUG  // TODO(bkuolt)
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
    static vec2 _current_angle {};
   _current_angle +=  angle;
    std::cout << "ANgle: " << _current_angle.x << ", " << _current_angle.y << std::endl;

    auto position =  glm::rotate(getPosition(), glm::radians(_current_angle.y), _up);
    setPosition(position);
}

mat4 Camera::getMatrix() const noexcept {
    return _P  *_V;
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

}  // namespace bgl
