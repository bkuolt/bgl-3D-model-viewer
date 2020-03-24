// Copyright 2020 Bastian Kuolt
#include "texture.hpp"

#include <iostream>
#include <mutex>  // std::once_flag
#include <string>

#include <IL/il.h>


namespace bgl {

Texture::Texture(const GLbyte *data, GLsizei width, GLsizei height) {
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenTextures(1, &_handle);
    if (_handle == 0 || glGetError() != GL_NO_ERROR) {
        throw std::invalid_argument("could not create GL texture");
    }

    bind();
    load(data, width, height);
    unbind();
}

Texture::~Texture() noexcept {
    glDeleteTextures(1, &_handle);
}

void Texture::bind() noexcept {
    glBindTexture(GL_TEXTURE_2D, _handle);
}

void Texture::unbind() noexcept {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::load(const GLbyte *data, GLsizei width, GLsizei height) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 width , height, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);

    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not upload texture data" };
    }
    glGenerateMipmap(GL_TEXTURE_2D);
    unbind();
    std::cout << "loaded texture" << std::endl;
}

/////////////////////////////////////////////////////////////////

namespace {

std::once_flag initialization_flag;

void initializeDevIL() {
    ilInit();
    if (ilGetError() != IL_NO_ERROR) {
        throw std::runtime_error { " could not initialize DevIL" };
    }
    // TODO(bkuolt): print IL version
    std::atexit(ilShutDown);
}

}  // namespace


SharedTexture LoadTexture(const std::filesystem::path &path) {
    std::call_once(initialization_flag, initializeDevIL);

    const ILuint handle = ilGenImage();
    ilBindImage(handle);

    if (!std::filesystem::exists(path)) {
        throw std::runtime_error { path.string() + " does not exist" };
    }

    if (!ilLoadImage(path.string().c_str())) {
        throw std::runtime_error { "could not load image" };
    }

    if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
        ilDeleteImage(handle);
        throw std::runtime_error { "could not convert image" };
    }

    auto image = std::make_shared<Texture>((GLbyte*)ilGetData(),
                                           ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
    ilDeleteImage(handle);
    return image;
}

}  // namespace bgl
