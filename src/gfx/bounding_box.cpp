#include "gfx/bounding_box.hpp"

namespace bgl {

BoundingBox::BoundingBox(float size) : BoundingBox({}, vec3{size}) {}

BoundingBox::BoundingBox(const vec3& center, const vec3& size) : _center(center), _size(size) {}

vec3 BoundingBox::getSize() const noexcept {
    return _size;
}

vec3 BoundingBox::getCenter() const noexcept {
    return _center;
}

void BoundingBox::resize(const vec3& size) noexcept {
    _size = size;
}

void BoundingBox::setCenter(const vec3& center) noexcept {
    _center = center;
}

}  // namespace bgl
