// Copyright 2020 Bastian Kuolt
#ifndef GFX_MESH_HPP
#define GFX_MESH_HPP

#include "gl.hpp"
#include "buffer.hpp"
#include "shader.hpp"

#include <filesystem>
#include <memory>

#include <assimp/texture.h>  // TODO(bkuolt): remove dependency

namespace bgl {

class Texture {
 public:
    explicit Texture(const aiTexture *texture);
    Texture(Texture&&) = delete;
    Texture(const Texture&) = delete;
    virtual ~Texture() noexcept;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    void bind() noexcept;
    void unbind() noexcept;

 private:
    void load(const aiTexture *texture);

    GLuint _handle;
};

using SharedTexture = std::shared_ptr<Texture>;  // TODO(bkuolt): implement

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
