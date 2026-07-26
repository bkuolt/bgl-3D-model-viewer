#pragma once

/**
 * @file bounding_box.hpp
 * @brief Axis-aligned bounding box representation for 3D objects.
 */

#include "gfx/math.hpp"

namespace bgl {

class BoundingBox {
  public:
    explicit BoundingBox(float size = 1.0f);
    BoundingBox(const vec3& center, const vec3& size);

    [[nodiscard]] vec3 getSize() const noexcept;
    [[nodiscard]] vec3 getCenter() const noexcept;

    void resize(const vec3& size) noexcept;
    void setCenter(const vec3& center) noexcept;

  private:
    vec3 _center{0.0f};
    vec3 _size{1.0f};
};

}  // namespace bgl
