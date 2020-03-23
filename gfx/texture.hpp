// Copyright 2020 Bastian Kuolt
#ifndef GFX_TEXTURE_HPP_
#define GFX_TEXTURE_HPP_

#include "gl.hpp"

#include <memory>
#include <filesystem>

namespace bgl {

class Texture {
 public:
    explicit Texture(const GLbyte *data, GLsizei width, GLsizei height);
    Texture(Texture&&) = delete;
    Texture(const Texture&) = delete;
    virtual ~Texture() noexcept;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    void bind() noexcept;
    void unbind() noexcept;

 private:
    void load(const GLbyte *data, GLsizei width, GLsizei height);

    GLuint _handle;
};

using SharedTexture = std::shared_ptr<Texture>;
SharedTexture LoadTexture(const std::filesystem::path &path);

}  // namespace bgl

#endif  // GFX_TEXTURE_HPP_
