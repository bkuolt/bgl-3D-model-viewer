// Copyright 2020 Bastian Kuolt
#include "../App.hpp"
#include "camera.hpp"

#include <stdexcept>


namespace bgl {

namespace {

double calculate_aspect_ratio() {
    const uvec2 size { App.window->getSize() };
    return size.y ? static_cast<double>(size.x) / size.y :  throw std::runtime_error { "invalid aspect ratio" };
}

}  // namespace

Camera::Camera(const vec3 &position, const vec3 &center)
    : _position { position }, _center { center }
{}

void Camera::setPosition(const vec3 &position) noexcept {
    _position = position;
}

void Camera::setZoom(double factor) {
    if (factor <= 0.0) {
        throw std::invalid_argument { "invalid zoom factor" };
    }
    _zoom = factor;
}

void Camera::setViewCenter(const vec3 &center) noexcept {
    _center = center;
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

mat4 Camera::getMatrix() const noexcept {
    // calculates the projection matrix @p P
    const double ratio = calculate_aspect_ratio();
    const mat4 P = glm::frustum(-(ratio / 2) * _zoom, (ratio / 2) *  _zoom,
                                 -_zoom,  _zoom,
                                 1.0, 10.0);

    // calculates the view matrix @p V
    const mat4 V = glm::lookAt(_position, _center, { 0.0, 1.0, 0.0 });

    return P * V;
}

void Camera::rotate(const vec2 degrees) noexcept {
    const vec2 currentAngle { glm::radians(glm::atan(_position.x)) };
    const double radius = glm::length(_position - _center);
    const double angle = glm::radians(currentAngle.x + degrees.x);  // TODO(bkuolt): incorporate y-axis
    setPosition({ radius * glm::cos(angle), 0.0f, radius * sin(angle) });
}

}  // namespace bgl
