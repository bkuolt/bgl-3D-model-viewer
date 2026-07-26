#pragma once

/**
 * @file box.hpp
 * @brief OpenGL wireframe bounding box renderable object.
 */

#include "gfx/gl.hpp"
#include "gfx/model.hpp"

namespace bgl {

/**
 * @brief Represents a renderable 3D wireframe bounding box.
 */
class Box final : public Model {
  public:
    Box();
    explicit Box(const BoundingBox& boundingBox);

    void render(const mat4& VP) override;
};

}  // namespace bgl
