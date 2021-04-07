#ifndef GFX_BOUNDING_BOX_HPP
#define GFX_BOUNDING_BOX_HPP

#include "math.hpp"


namespace bgl {

class Ray {
	// TODO
};

class BoundingVolume {
 public:
	explicit BoundingVolume(const vec3 &center = {}) noexcept;
	virtual ~BoundingVolume() noexcept = default;

	virtual bool liesInside(const vec3 &point) const noexcept = 0;
	virtual bool hits(const Ray &ray) const noexcept = 0;

	void translate(const vec3 &v) noexcept;

	void setCenter(const vec3 &center);
	const vec3& getCenter() const noexcept;

 private:
	vec3 _center;
};

class BoundingBox : public BoundingVolume {
 public:
	explicit BoundingBox(const vec3 &size = vec3 { 1.0f });
	explicit BoundingBox(const vec3 &center, const vec3 &size = vec3 { 1.0f });

	virtual ~BoundingBox() noexcept = default;

	template<typename InputIterator>
	BoundingBox(InputIterator first, InputIterator last) {
		// TODO
	}

	bool liesInside(const vec3 &point) const noexcept override;
	bool hits(const Ray &ray) const noexcept override;

	void resize(const vec3 &size);
	vec3 getSize() const noexcept;

 private:
	vec3 _size;
};

class BoundingSphere : public BoundingVolume {
 public:
	explicit BoundingSphere(float radius = 1.0f);
	explicit BoundingSphere(const vec3 &position, float radius = 1.0f);

	virtual ~BoundingSphere() noexcept = default;

	template<typename InputIterator>
	BoundingSphere(InputIterator first, InputIterator last) {
		// TODO
	}

	bool liesInside(const vec3 &point) const noexcept override;
	bool hits(const Ray &ray) const noexcept override;

	void resize(float radius);
	float getSize() const noexcept;

 private:
	float _radius;
};

bool collides(const BoundingBox &lhs, const BoundingBox &rhs) noexcept;
bool collides(const BoundingSphere &lhs, const BoundingSphere &rhs) noexcept;
bool collides(const BoundingSphere &lhs, const BoundingBox &rhs) noexcept;

}  // namespace bgl

#endif  // GFX_BOUNDING_BOX_HPP
