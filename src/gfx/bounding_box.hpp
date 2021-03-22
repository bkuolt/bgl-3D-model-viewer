#ifndef GFX_BOUNDING_BOX_HPP
#define GFX_BOUNDING_BOX_HPP

#include "gl.hpp"


namespace bgl {

struct Bound {
   float min;
   float max;
};

struct BoundingBox final {
	BoundingBox() = default;
	BoundingBox(const vec3 &center, const vec3 &size) {
		// TODO(bkuolt)
	}

	explicit BoundingBox(const vec3 &size)
	 : BoundingBox({}, size) {}

	void resize(const vec3 &size) noexcept {
		_size = size;
	}

	const vec3& getSize() const noexcept {
		_size = vec3 {
			_bounds.x.max -  _bounds.x.min,
			_bounds.y.max -  _bounds.y.min,
			_bounds.z.max -  _bounds.z.min
		};
		return _size;
	}

	tvec3<Bound>& getBounds() noexcept {
		return _bounds;
	}

	const tvec3<Bound>& getBounds() const noexcept {
		return _bounds;
	}

 // TODO(bkuolt: private:
	mutable vec3 _size;
	tvec3<Bound> _bounds;
};

}  // namespace bgl

#endif  // GFX_BOUNDING_BOX_HPP
