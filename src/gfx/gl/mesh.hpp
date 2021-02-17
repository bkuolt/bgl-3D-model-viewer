// Copyright 2020 Bastian Kuolt
#ifndef GFX_MESH_HPP
#define GFX_MESH_HPP

#include "gl.hpp"
#include "buffer.hpp"

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include "box.hpp"

#include <iostream>
#include <filesystem>
#include <memory>
#include <string>


namespace bgl {


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
    SharedVBO<Vertex> _vbo;
    SharedIBO _ibo;
    SharedVAO<Vertex> _vao;
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
