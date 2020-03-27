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


#if 1
class Box {
 public:
    using SharedVBO = bgl::SharedVBO<vec3>;
    using SharedVAO = bgl::SharedVAO<vec3>;

    explicit Box(GLfloat size = 1);
    explicit Box(const vec3 &dimensions);

    void render(const mat4 &VP);
    void resize(const vec3 &dimensions);

 private:
    vec3 _dimensions;

    SharedVBO _vbo;
    SharedIBO _ibo;
    SharedVAO _vao;
    SharedProgram _program;
};
#endif  // 1

class Mesh {
 public:
    explicit Mesh(const std::filesystem::path &path);
    virtual ~Mesh() = default;
    void render(const mat4 &MVP);

 private:
    SharedVBO<Vertex> _vbo;
    SharedIBO _ibo;
    SharedVAO<Vertex> _vao;
    SharedTexture _texture;
    SharedProgram _program;
    Box _box;
};

using SharedMesh = std::shared_ptr<Mesh>;

inline SharedMesh LoadMesh(const std::filesystem::path &path) {
    return std::make_shared<Mesh>(path);
}

}  // namespace bgl

#endif  // GFX_MESH_HPP
