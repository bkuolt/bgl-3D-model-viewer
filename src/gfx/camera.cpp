#include <cassert>
#include <cmath>
#include <iostream>
#include <stdexcept>

#include "camera.hpp"


namespace bgl {

namespace {

float calculate_aspect_ratio() {
#if 1
    return 16.0f / 9.0f;
#else
    const uvec2 size { () ? App.window->getSize() : throw std::runtime_error { "could not get aspect ratio" }};
    return size.y ? static_cast<double>(size.x) / size.y : throw std::runtime_error { "invalid aspect ratio" };
#endif  // DEBUG
}

}  // anonymous namespace

/* ----------------------------- Camera ----------------------------- */

Camera::Camera(const vec3 &position, const vec3 &focus)
    : _position { position } {
    setFocus(focus);
}

void Camera::setPosition(const vec3 &position) noexcept {
    _position = position;
}

void Camera::setFocus(const vec3 &focus) {
    if (_position == focus) {
        throw std::invalid_argument { "invalid camera focus" };
    }
    _center = focus;
}

void Camera::setZoom(float factor) {
    if (factor <= 0) {
        throw std::invalid_argument { "invalid zoom factor" };
    }
    _zoom = factor;
}

const vec3& Camera::getPosition() const noexcept {
    return _position;
}

const vec3& Camera::getFocus() const noexcept {
    return _center;
}

float Camera::getZoom() const noexcept {
    assert(_zoom > 0);
    return _zoom;
}

mat4 Camera::matrix() const noexcept {
    assert(_zoom > 0);

    const float ratio { calculate_aspect_ratio() };
    const float width { (ratio / 2) * _zoom };
    const mat4 P {
        glm::frustum(-width, width, -_zoom,  _zoom, 1.0f, 10.0f)
    };
    const mat4 V {
        glm::lookAt(_position * _zoom, _center, _up)
    };

    return P * V;
}

void Camera::setUp(const vec3 &up) {
    if (up == vec3 {}) {
        throw std::invalid_argument { "invalid up vector" };
    }
    _up = up;
}

void Camera::translate(const vec3 &v) noexcept {
    _position += v;
}

const vec3& Camera::getUp() const noexcept {
    return _up;
}

/* ----------------------------- ArcBall ----------------------------- */

ArcBall::ArcBall(const vec3 &position, float radius,
                 float theta /* θ [°]*/, float phi /* φ [°] */)
        : Camera(position, position + vec3 { 0, 0, radius }),
          _angles { theta, phi } {
}

void ArcBall::rotate(float theta  /* θ [°]*/,
                     float phi /* φ [°]*/) {
    _angles.theta += glm::radians(theta);
    _angles.phi += glm::radians(phi);

    _angles.theta = std::fmod(_angles.theta, M_PI * 2);
    _angles.phi = std::fmod(_angles.phi, M_PI * 2);

    const float radius { glm::distance(getPosition(), getFocus()) };
    const vec3 v {
        std::cos(_angles.phi) * std::sin(_angles.theta),
        std::sin(_angles.phi) * std::sin(_angles.theta),
        std::cos(_angles.theta)
    };  // https://en.wikipedia.org/wiki/Spherical_coordinate_system

    setPosition(getFocus() + (v * radius));
}

vec2 ArcBall::getAngles() const noexcept {
    return { _angles.theta, _angles.phi };
}

}  // namespace bgl
