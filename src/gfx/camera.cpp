#include "gfx/camera.hpp"

#include <cassert>
#include <cmath>
#include <numbers>
#include <stdexcept>

namespace bgl {

namespace {

constexpr float calculate_aspect_ratio() noexcept {
    return 16.0f / 9.0f;
}

}  // anonymous namespace

/* ----------------------------- Camera ----------------------------- */

Camera::Camera(const vec3& position, const vec3& focus) : _position{position} {
    setFocus(focus);
}

void Camera::setPosition(const vec3& position) noexcept {
    _position = position;
}

void Camera::setFocus(const vec3& focus) {
    if (_position == focus) {
        throw std::invalid_argument{"invalid camera focus position"};
    }
    _center = focus;
}

void Camera::setZoom(float factor) {
    if (factor <= 0.0f) {
        throw std::invalid_argument{"invalid camera zoom factor"};
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
    assert(_zoom > 0.0f);
    return _zoom;
}

mat4 Camera::matrix() const noexcept {
    assert(_zoom > 0.0f);

    const float ratio{calculate_aspect_ratio()};
    const float width{(ratio / 2.0f) * _zoom};
    const mat4 P{glm::frustum(-width, width, -_zoom, _zoom, 1.0f, 10.0f)};
    const mat4 V{glm::lookAt(_position * _zoom, _center, _up)};

    return P * V;
}

void Camera::setUp(const vec3& up) {
    if (up == vec3{}) {
        throw std::invalid_argument{"invalid up vector"};
    }
    _up = up;
}

void Camera::translate(const vec3& v) noexcept {
    _position += v;
}

const vec3& Camera::getUp() const noexcept {
    return _up;
}

/* ----------------------------- ArcBall ----------------------------- */

ArcBall::ArcBall(const vec3& position, float radius, float theta, float phi)
    : Camera(position, position + vec3{0.0f, 0.0f, radius}), _angles{phi, theta} {}

void ArcBall::rotate(float thetaDegrees, float phiDegrees) {
    constexpr auto two_pi = static_cast<float>(std::numbers::pi * 2.0);

    _angles.theta += glm::radians(thetaDegrees);
    _angles.phi += glm::radians(phiDegrees);

    _angles.theta = std::fmod(_angles.theta, two_pi);
    _angles.phi = std::fmod(_angles.phi, two_pi);

    const float radius{glm::distance(getPosition(), getFocus())};
    const vec3 v{std::cos(_angles.phi) * std::sin(_angles.theta),
                 std::sin(_angles.phi) * std::sin(_angles.theta), std::cos(_angles.theta)};

    setPosition(getFocus() + (v * radius));
}

vec2 ArcBall::getAngles() const noexcept {
    return {_angles.theta, _angles.phi};
}

}  // namespace bgl
