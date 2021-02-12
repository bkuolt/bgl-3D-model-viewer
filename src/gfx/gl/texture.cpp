// Copyright 2020 Bastian Kuolt
#include "texture.hpp"

#include <iostream>
#include <mutex>  // std::once_flag
#include <string>


#include<SDL2/SDL_video.h>
#include <IL/il.h>


namespace bgl {

/* ---------------------- IL Helper ---------------------- */

namespace {
namespace il {

std::once_flag init_flag;

void Init() {
    ilInit();
    if (ilGetError() != IL_NO_ERROR) {
        throw std::runtime_error { " could not initialize DevIL" };
    }
    // TODO(bkuolt): print IL version
    std::atexit(ilShutDown);
}

ILuint GenImage() {
    const ILuint handle { ilGenImage() };
    return (ilGetError() == IL_NO_ERROR) ? handle
                                         : throw std::runtime_error { "could not create a DevIL image" };
}

}  // namespace il
}  // namespace

/* --------------------------------- Texture ---------------------------------- */

void initializeTexturing() {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::Texture() {
    initializeTexturing();
    glGenTextures(1, &_handle);
}

Texture::Texture(GLuint handle)
    : _handle { handle }
{
    initializeTexturing();
    bind();
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &_size.x);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &_size.y);
    unbind();
}

Texture::Texture(const GLbyte *data, GLsizei width, GLsizei height) {
    initializeTexturing();

    glGenTextures(1, &_handle);
    if (_handle == 0 || glGetError() != GL_NO_ERROR) {
        throw std::invalid_argument("could not create GL texture");
    }
    load(data, width, height);
}

Texture::Texture(GLsizei width, GLsizei height)
    : Texture(nullptr, width, height) {
}

Texture::~Texture() noexcept {
    glDeleteTextures(1, &_handle);
}

void Texture::bind() const noexcept {
    glBindTexture(GL_TEXTURE_2D, _handle);
}

void Texture::unbind() const noexcept {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::load(const GLbyte *data, GLsizei width, GLsizei height) {
    bind();
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width , height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (glGetError() != GL_NO_ERROR) {
        unbind();
        throw std::runtime_error { "could not upload texture data" };
    }

    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
    _size = uvec2 { width, height };
    std::cout << "texture size: "
              << ((width * height * 4) * sizeof(GLubyte)) / 1'000'000.0 << " MB"
              << std::endl;
}

std::vector<GLubyte> Texture::getPixels() const {
    bind();
    std::vector<GLubyte> buffer(_size.x * _size.y * 3);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
    unbind();
    return buffer;
}

uvec2 Texture::getSize() const noexcept {
    return _size;
}

GLuint Texture::getHandle() const noexcept {
    return _handle;
}

/* ---------------------------------- Helper ---------------------------------- */

SharedTexture LoadTexture(const std::filesystem::path &path) {
    std::call_once(il::init_flag, il::Init);

    const ILuint handle { il::GenImage() };
    ilBindImage(handle);

    if (!std::filesystem::exists(path)) {
        throw std::runtime_error { path.string() + " does not exist" };
    }

    const ILboolean could_load { ilLoadImage(path.string().c_str()) };
    if (!could_load) {
        ilDeleteImage(handle);
        throw std::runtime_error { "could not load image" };
    }

    const ILboolean could_convert { ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE) };
    // TODO(bkuolt): fix this memory leak ("ilDeleteImage(handle);")"
    if (!could_convert) {
        throw std::runtime_error { "could not convert image" };
    }

    return std::make_shared<Texture>(reinterpret_cast<GLbyte*>(ilGetData()),
                                     ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
}

void SaveTexture(const SharedTexture &texture, const std::filesystem::path &path) {
    std::call_once(il::init_flag, il::Init);

    const uvec2 size { texture->getSize() };
    std::vector<GLubyte> pixels { texture->getPixels() };

    // saves texture to a file
    const ILuint image { il::GenImage() };
    ilBindImage(image);
    ilTexImage(size.x, size.y, 0, 3, IL_RGB, IL_UNSIGNED_BYTE, pixels.data() );
    const ILboolean successfull { ilSaveImage(path.string().c_str()) };
    ilDeleteImage(image);
    if (!successfull) {
        throw std::runtime_error { "could not save image file" };
    }
}

}  // namespace bgl
