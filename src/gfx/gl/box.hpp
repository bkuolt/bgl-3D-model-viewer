// Copyright 2021 Bastian Kuolt
#ifndef GL_BOX_HPP_
#define GL_BOX_HPP_

#include "gl.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include <assimp/mesh.h>  // TODO(bkuolt)

namespace bgl {

struct bounding_box {
    float min;
    float max;
};

glm::tvec3<bounding_box> get_bounds(const aiMesh &mesh) noexcept;

class Box final {
 public:
    explicit Box(GLfloat size = 1);
    explicit Box(const vec3 &dimensions);

    void render(const mat4 &VP);
    void resize(const vec3 &dimensions);

    const vec3& getSize() const noexcept {
       return _dimensions;
    }

 private:
    using SharedVBO = bgl::SharedVBO<vec3>;
    using SharedVAO = bgl::SharedVAO<vec3>;

    vec3 _dimensions;

    SharedVBO _vbo;
    SharedIBO _ibo;
    SharedVAO _vao;
    SharedProgram _program;
};

}  // namespace bgl
#endif  // GL_BOX_HPP_
