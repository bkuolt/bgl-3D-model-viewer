#ifndef GFX_CAMERA_HPP_
#define GFX_CAMERA_HPP_

#include "math.hpp"


namespace bgl {

class Camera {
 public:
	Camera() noexcept {};
	Camera(const Camera&) = default;
	Camera(Camera&&) = default;

	Camera(const vec3 &position, const vec3 &focus);

	virtual ~Camera() noexcept = default;

	Camera& operator=(const Camera&) = default;
	Camera& operator=(Camera&&) = default;

	void setPosition(const vec3 &position) noexcept;
	void setFocus(const vec3 &focus);
	void setZoom(float factor);
	void setUp(const vec3 &up);

	void translate(const vec3 &v) noexcept;

	const vec3& getPosition() const noexcept;
	const vec3& getFocus() const noexcept;
	float getZoom() const noexcept;
	const vec3& getUp() const noexcept;

	mat4 matrix() const noexcept;

 private:
	vec3 _position { 0.0, 0.0, 1.0 };
	vec3 _center { 0.0, 0.0, 0.0 };
	vec3 _up { 0.0, 1.0, 0.0 };
	float _zoom { 1.0 };
};

class ArcBall : public Camera {
 public:
	ArcBall() noexcept {};
	ArcBall(const vec3 &position, float radius,
			float theta /* θ [°]*/ = 0, float phi /* φ [°] */ = 0);

	virtual ~ArcBall() noexcept = default;

	void rotate(float theta  /* θ [°]*/, float phi /* φ [°]*/);

	vec2 getAngles() const noexcept;
	// TODO: get radius
	// TODO: set radius

 private:
	struct {
		float phi { 0 };
		float theta { M_PI / 2 };
	} _angles;
};

}  // namespace bgl

#endif  // GFX_CAMERA_HPP_
