// Copyright 2020 Bastian Kuolt
#ifndef GFX_MESH_HPP
#define GFX_MESH_HPP

#include "gl.hpp"
#include "buffer.hpp"
#include "shader.hpp"

#include <filesystem>
#include <memory>


namespace bgl {

using SharedTexture = std::shared_ptr<GLuint>;  // TODO(bkuolt): implement

class Mesh {
 public:
    explicit Mesh(const std::filesystem::path &path);
    virtual ~Mesh() = default;
    void render(const mat4 &MVP);

 private:
    SharedVBO _vbo;
    SharedIBO _ibo;
    SharedVAO _vao;
    SharedTexture _texture;
    SharedProgram _program;
};

using SharedMesh = std::shared_ptr<Mesh>;

inline SharedMesh LoadMesh(const std::filesystem::path &path) {
    return std::make_shared<Mesh>(path);
}

}  // namespace bgl

#endif  // GFX_MESH_HPP
