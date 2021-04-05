#ifndef GFX_CAMERA_HPP_
#define GFX_CAMERA_HPP_

#include "gl.hpp"


namespace bgl {

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

	enum class RotationAxis { Y, Z };

	void rotate(float angle, RotationAxis axis);

	void zoom(float delta) {
		// TODO
	}

	const vec3& getPosition() const noexcept;
	const vec3& getViewCenter() const noexcept;
	float getZoom() const noexcept;

	mat4 getMatrix() const noexcept;

 private:
    void updateProjectionMatrix();
    void updateViewMatrix() noexcept;

    vec3 _position { 0.0, 0.0, 1.0 };
    vec3 _center { 0.0, 0.0, 0.0 };
    const vec3 _up {0.0, 1.0, 0.0 };
    float _zoom { 1.0 };

    mat4 _V;
    mat4 _P;
    vec2 _angle {0, 90 };
};

}  // namespace bgl

#endif  // GFX_CAMERA_HPP_
