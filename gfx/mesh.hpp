// Copyright 2020 Bastian Kuolt
#ifndef GFX_MESH_HPP
#define GFX_MESH_HPP

#include "gl.hpp"
#include "buffer.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include <filesystem>
#include <memory>

#include <assimp/texture.h>  // TODO(bkuolt): remove dependency

namespace bgl {

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
