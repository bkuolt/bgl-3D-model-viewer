// Copyright 2021 Bastian Kuolt
#ifndef GFX_GRID_HPP
#define GFX_GRID_HPP

#include "mesh.hpp"

namespace bgl {

// TODO(bkuolt): move to separate header
class Grid final : public Mesh {
 public:
    explicit Grid(GLfloat size, std::size_t num_cells);
    void render(const mat4 &MVP) override;
    void translate(const vec3 &v);

 private:
    void create_vbo();
    void create_ibo();
    void create_vao();

    const GLfloat _cell_size;
    const std::size_t _num_cells;
    vec3 _translation;
};

}  // namespace bgl

#endif  // GFX_GRID_HPP
