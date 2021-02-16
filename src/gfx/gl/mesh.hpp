// Copyright 2020 Bastian Kuolt
#ifndef GFX_MESH_HPP
#define GFX_MESH_HPP

#include "gl.hpp"
#include "box.hpp"

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>


#include <iostream>
#include <filesystem>
#include <memory>
#include <string>


namespace bgl {

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoords;
};

class 
Mesh {
 public:
    explicit Mesh(const std::filesystem::path &path);
    virtual ~Mesh() = default;
    void render(const mat4 &MVP);

    const Box& getBoundingBox() const noexcept {
       return _box;
    }


 private:
   std::shared_ptr<VertexArrayObject> _vao;
   std::shared_ptr<QOpenGLBuffer> _vbo;
   std::shared_ptr<QOpenGLBuffer> _ibo;
   std::shared_ptr<QOpenGLTexture> _texture;
   std::shared_ptr<QOpenGLShaderProgram> _program;
   Box _box;
};

using SharedMesh = std::shared_ptr<Mesh>;

inline SharedMesh LoadMesh(const std::string &path) {
   std::cout << "LoadMesh()" << std::endl;
     auto r = std::shared_ptr<bgl::Mesh>(new bgl::Mesh(path));
   std::cout << "Done loading" << std::endl;
   return r;
}

}  // namespace bgl

#endif  // GFX_MESH_HPP
