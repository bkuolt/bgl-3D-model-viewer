#ifndef GFX_BOUNDING_BOX_HPP
#define GFX_BOUNDING_BOX_HPP

#include "math.hpp"


namespace bgl {

class BoundingBox {
 public:
	BoundingBox(const vec3 &center, const vec3 &size)
		: _center(center), _size(size) {
	}

	explicit BoundingBox(float size = 1.0f)
		: BoundingBox({}, vec3 { 1.0f } ) {
	}

	vec3 getSize() const noexcept {
		return _size;
	}

	void resize(const vec3 &size) {
		_size = size;
	}

 private:
	vec3 _center;
	vec3 _size;
};

}  // namespace bgl

#endif  // GFX_BOUNDING_BOX_HPP
