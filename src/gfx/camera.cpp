#include <stdexcept>
#include <iostream>

#include "camera.hpp"
#define DEBUG  // TODO(bkuolt)


namespace bgl {

namespace {

double calculate_aspect_ratio() {
#ifdef DEBUG
    return 16.0 / 9.0;
#else
    const uvec2 size { () ? App.window->getSize() : throw std::runtime_error { "could not get aspect ratio" }};
    return size.y ? static_cast<double>(size.x) / size.y : throw std::runtime_error { "invalid aspect ratio" };
#endif  // DEBUG
}

}  // anonymous namespace


Camera::Camera() noexcept {
    updateProjectionMatrix();
}

Camera::Camera(const vec3 &position, const vec3 &center)
    : _position { position }, _center { center } {
    updateProjectionMatrix();
    updateViewMatrix();
}

void Camera::updateProjectionMatrix() {
    const float ratio = calculate_aspect_ratio();
    const float width { (ratio / 2) * _zoom };
    _P = glm::frustum(-width, width,
                      -_zoom,  _zoom,
                      1.0f, 10.0f);
}

void Camera::updateViewMatrix() noexcept {
    _V = glm::lookAt(_position * _zoom, _center, _up);
}

void Camera::rotate(float angle, RotationAxis axis) {
    _angle += angle;

    vec3 v;
    if (axis == RotationAxis::Y) {
        v.x = std::cos(glm::radians(_angle.y));
        v.z = std::sin(glm::radians(_angle.y));
    }

    // v.y = std::sin(glm::radians(_angle.x));
    // v.z += std::cos(glm::radians(_angle.x));

    auto distance = glm::distance(_position, _center);
    _position = _center + (v * distance);
    updateViewMatrix();
}

mat4 Camera::getMatrix() const noexcept {
    return _P  *_V;
}

void Camera::setPosition(const vec3 &position) noexcept {
    _position = position;
    updateViewMatrix();
}

void Camera::setZoom(double factor) {
    if (factor <= 0) return;

    _zoom = (factor > 0.0) ? factor : throw std::invalid_argument { "invalid zoom factor" };

    std::cout << "set zoom to "<< _zoom << std::endl;
    updateProjectionMatrix();
    updateViewMatrix();
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

float Camera::getZoom() const noexcept {
    return _zoom;
}

}  // namespace bgl
