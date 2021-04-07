#include "bounding_box.hpp"


namespace bgl {

BoundingBox::BoundingBox(float size)
    : BoundingBox({}, vec3 { 1.0f } ) {
}

BoundingBox::BoundingBox(const vec3 &center, const vec3 &size)
    : _center(center), _size(size) {
}

vec3 BoundingBox::getSize() const noexcept {
    return _size;
}

void BoundingBox::resize(const vec3 &size) {
    _size = size;
}

}  // namespace bgl

