// Copyright 2020 Bastian Kuolt
#ifndef GFX_CAMERA_HPP_
#define GFX_CAMERA_HPP_

#include "gl.hpp"

#include <boost/timer/timer.hpp>
#include <chrono>
#include <memory>

namespace bgl {
class camera_motion;

class Camera {
 public:
    Camera() noexcept;
    Camera(const vec3 &position, const vec3 &viewCenter);
    Camera(const Camera&) = default;
    Camera(Camera&&) = default;

    virtual ~Camera() noexcept = default;

    Camera& operator=(const Camera&) = default;
    Camera& operator=(Camera&&) = default;

    void setPosition(const vec3 &position) noexcept;
    void setZoom(double factor = 1.0);
    void setViewCenter(const vec3 &center) noexcept;
    void rotate(const vec2 degrees) noexcept;

    const vec3& getPosition() const noexcept;
    const vec3& getViewCenter() const noexcept;
    double getZoom() const noexcept;

    mat4 getMatrix() const noexcept;

    enum class horizontal_direction { left, right };
    using shared_motion = std::shared_ptr<camera_motion>;
    shared_motion createMotion(horizontal_direction direction, float speed);

 private:
    friend class camera_motion;
    void updateProjectionMatrix();
    void updateViewMatrix() noexcept;

    vec3 _position { 0.0, 0.0, 1.0 };
    vec3 _center { 0.0, 0.0, 0.0 };
    const vec3 _up {0.0, 1.0, 0.0 };
    double _zoom { 1.0 };

    mat4 _V;
    mat4 _P;
};

#if 1
class timer {
 public:
    void start() noexcept {
        _timer.start();
    }

    void stop() noexcept {
        _timer.stop();
    }

    bool is_running() const noexcept {
       return !_timer.is_stopped();
    }

    std::chrono::milliseconds elapsed() const noexcept {
        const boost::timer::cpu_times times { _timer.elapsed() };
        const std::chrono::nanoseconds nanoseconds { times.system + times.user + times.wall };
        return std::chrono::duration_cast<std::chrono::milliseconds>(nanoseconds);
    }

 private:
    boost::timer::cpu_timer _timer;
};

#endif  // 1

class camera_motion {
 public:
    camera_motion(Camera &camera, const vec3 &axis, double speed);
    void start() noexcept;
    void stop() noexcept;
    bool is_running() const noexcept;

    void update() noexcept;
 private:
    Camera& _camera;
    const vec3 _axis;    // the rotation axis
    const double _speed;  // [°/s]

    timer _timer;
    std::chrono::milliseconds _timestamp {};
};

}  // namespace bgl

#endif  // GFX_CAMERA_HPP_