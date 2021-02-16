// Copyright 2021 Bastian Kuolt
#ifndef GL_BOX_HPP_
#define GL_BOX_HPP_

#include "gl.hpp"

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <assimp/mesh.h>  // TODO(bkuolt)
#include <memory>


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
   vec3 _dimensions;

   std::shared_ptr<QOpenGLVertexArrayObject> _vao;
   std::shared_ptr<QOpenGLBuffer> _vbo;
   std::shared_ptr<QOpenGLBuffer> _ibo;

   std::shared_ptr<QOpenGLShaderProgram> _program;
};

}  // namespace bgl
#endif  // GL_BOX_HPP_
