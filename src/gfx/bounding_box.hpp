#ifndef GFX_BOUNDING_BOX_HPP
#define GFX_BOUNDING_BOX_HPP

#include "math.hpp"


namespace bgl {

class BoundingBox {
 public:
	explicit BoundingBox(float size = 1.0f);
	BoundingBox(const vec3 &center, const vec3 &size);

	vec3 getSize() const noexcept;
	void resize(const vec3 &size);
	// TODO: getCenter()
	// TODO: setCenter()
	// TODO: translate()
	// TODO: collides()

 private:
	vec3 _center;
	vec3 _size;
};

}  // namespace bgl

#endif  // GFX_BOUNDING_BOX_HPP
