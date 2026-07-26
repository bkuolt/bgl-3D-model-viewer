#pragma once

/**
 * @file grid.hpp
 * @brief Renderable 3D reference grid overlay.
 */

#include "gfx/model.hpp"

namespace bgl {

class Grid final : public Model {
  public:
    Grid(GLfloat size, std::size_t num_cells);

    void render(const mat4& MVP) override;
    void translate(const vec3& v);

  private:
    void create_vbo();
    void create_ibo();
    void create_vao();

    GLfloat _cell_size;
    std::size_t _num_cells;
    vec3 _translation{0.0f};
};

}  // namespace bgl
