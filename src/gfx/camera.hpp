#pragma once

/**
 * @file camera.hpp
 * @brief Camera abstractions including 3D perspective and ArcBall orbit camera.
 */

#include <numbers>

#include "gfx/math.hpp"

namespace bgl {

/**
 * @brief Base 3D perspective camera class.
 */
class Camera {
  public:
    Camera() noexcept = default;
    Camera(const vec3& position, const vec3& focus);

    virtual ~Camera() noexcept = default;

    Camera(const Camera&) = default;
    Camera& operator=(const Camera&) = default;
    Camera(Camera&&) noexcept = default;
    Camera& operator=(Camera&&) noexcept = default;

    void setPosition(const vec3& position) noexcept;
    void setFocus(const vec3& focus);
    void setZoom(float factor);
    void setUp(const vec3& up);

    void translate(const vec3& v) noexcept;

    [[nodiscard]] const vec3& getPosition() const noexcept;
    [[nodiscard]] const vec3& getFocus() const noexcept;
    [[nodiscard]] float getZoom() const noexcept;
    [[nodiscard]] const vec3& getUp() const noexcept;

    [[nodiscard]] mat4 matrix() const noexcept;

  private:
    vec3 _position{0.0f, 0.0f, 1.0f};
    vec3 _center{0.0f, 0.0f, 0.0f};
    vec3 _up{0.0f, 1.0f, 0.0f};
    float _zoom{1.0f};
};

/**
 * @brief Orbital ArcBall camera implementation for model viewing.
 */
class ArcBall : public Camera {
  public:
    ArcBall() noexcept = default;
    ArcBall(const vec3& position, float radius, float theta = 0.0f, float phi = 0.0f);

    ~ArcBall() noexcept override = default;

    void rotate(float thetaDegrees, float phiDegrees);

    [[nodiscard]] vec2 getAngles() const noexcept;

  private:
    struct {
        float phi{0.0f};
        float theta{static_cast<float>(std::numbers::pi) / 2.0f};
    } _angles;
};

}  // namespace bgl
