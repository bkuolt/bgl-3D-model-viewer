// Copyright 2020 Bastian Kuolt
#include "texture.hpp"

#include <iostream>
#include <string>

#include <IL/il.h>


namespace bgl {

Texture::Texture(const GLbyte *data, GLsizei width, GLsizei height) {
    glGenTextures(1, &_handle);
    if (_handle == 0) {
        throw std::invalid_argument("");
    }

    bind();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (_handle == 0 || glGetError() != GL_NO_ERROR) {
        throw std::runtime_error("");
    }

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
    bind();
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

SharedTexture LoadTexture(const std::filesystem::path &path) {
    ilInit();
    if (ilGetError() != IL_NO_ERROR) {
        throw std::runtime_error { " could not initialize DevIL" };
    }
    std::atexit(ilShutDown);


    const ILuint handle = ilGenImage();
    ilBindImage(handle);

        //path.string().c_str());
    if (!ilLoadImage("/home/bastian/code/bgl-demo/assets/MedHouse/HouseMed_Diff.jpg")) {
        throw std::runtime_error { " could not load image" };
    }

    if (!ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE)) {
        ilDeleteImage(handle);
        throw std::runtime_error { " could not convert image" };
    }

    auto image = std::make_shared<Texture>((GLbyte*)ilGetData(),
                                           ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT));
    ilDeleteImage(handle);
    return image;
}

}  // namespace bgl
