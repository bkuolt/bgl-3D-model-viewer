#include "bounding_box.hpp"


namespace bgl {

/* -------------------------- BoundingVolume -------------------------- */

BoundingVolume::BoundingVolume(const vec3 &center) noexcept
    : _center { center } {
}

void BoundingVolume::translate(const vec3 &v) noexcept {
    // TODO
}

void BoundingVolume::setCenter(const vec3 &center) {
    // TODO
}

const vec3& BoundingVolume::getCenter() const noexcept {
    return _center;
}

/* --------------------------- BoundingBox --------------------------- */

BoundingBox::BoundingBox(const vec3 &size)
    : _size { size } {
}

BoundingBox::BoundingBox(const vec3 &center, const vec3 &size)
    : BoundingVolume { center }, _size { size } {
}

vec3 BoundingBox::getSize() const noexcept {
    return _size;
}

void BoundingBox::resize(const vec3 &size) {
    _size = size;
}

bool BoundingBox::liesInside(const vec3 &point) const noexcept {
    return false;  // TODO
}

bool BoundingBox::hits(const Ray &ray) const noexcept {
    return false;  // TODO
}

/* -------------------------- BoundingSphere ------------------------- */

BoundingSphere::BoundingSphere(float radius) {
    // TODO
}

BoundingSphere::BoundingSphere(const vec3 &position, float radius) {
    // TODO
}

float BoundingSphere::getSize() const noexcept {
    // TODO
    return _radius;
}

void BoundingSphere::resize(float radius) {
    // TODO
}

bool BoundingSphere::liesInside(const vec3 &point) const noexcept {
    return false;  // TODO
}

bool BoundingSphere::hits(const Ray &ray) const noexcept {
    return false;  // TODO
}

/* ------------------------------------------------------------------- */

bool collides(const BoundingBox &lhs, const BoundingBox &rhs) noexcept {
    return false;  // TODO
}

bool collides(const BoundingSphere &lhs, const BoundingSphere &rhs) noexcept {
    return false;  // TODO
}

bool collides(const BoundingSphere &lhs, const BoundingBox &rhs) noexcept {
    return false;  // TODO
}

}  // namespace bgl

