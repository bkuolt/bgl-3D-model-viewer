// Copyright 2020 Bastian Kuolt
#ifndef GFX_TEXTURE_HPP_
#define GFX_TEXTURE_HPP_

#include "gl.hpp"

#include <memory>
#include <filesystem>
#include <vector>

namespace bgl {

class Texture {
 public:
    Texture();
    explicit Texture(GLuint handle);
    explicit Texture(GLsizei width, GLsizei height);
    explicit Texture(const GLbyte *data, GLsizei width, GLsizei height);

    Texture(Texture&&) = delete;
    Texture(const Texture&) = delete;

    virtual ~Texture() noexcept;

    Texture& operator=(const Texture&) = delete;
    Texture& operator=(Texture&&) = delete;

    void bind() const noexcept;
    void unbind() const noexcept;

    std::vector<GLubyte> getPixels() const;
    uvec2 getSize() const noexcept;
    GLuint getHandle() const noexcept;

 private:
    void load(const GLbyte *data, GLsizei width, GLsizei height);

    GLuint _handle;
    ivec2 _size;
};

using SharedTexture = std::shared_ptr<Texture>;

/**
 * @brief Loads a texture from a given path.
 */
SharedTexture LoadTexture(const std::filesystem::path &path);

/**
 * @brief Saves a texture to a given path.
 */
void SaveTexture(const SharedTexture &texture, const std::filesystem::path &path);

/**
 * @brief Reads the color buffer and saves it in a Texture.
 */
SharedTexture GetColorBuffer();

}  // namespace bgl

#endif  // GFX_TEXTURE_HPP_
