// Copyright 2020 Bastian Kuolt
#include "shader.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


namespace bgl {

Shader::Shader(GLenum type, const std::filesystem::path & path)
    : _handle { glCreateShader(type) }, _type { type } {
    if (_handle == 0) {
        throw std::runtime_error { "could not create shader object"};
    }
    load(path);
    compile();
}

Shader::~Shader() noexcept {
    glDeleteShader(_handle);
}

void Shader::load(const std::filesystem::path &path) {
    std::vector<GLchar*> lines;

    std::ifstream file { path.string() };
    std::string string;
    std::getline(file, string);
    while (!file.eof()) {
        if (file.fail()) {
            throw std::runtime_error { "could not read shader source file " };
        }

        GLchar *line = strdup((std::string(string.c_str()) + "\n").c_str());
        lines.push_back(line);
        std::getline(file, string);
    }

    glShaderSource(_handle, lines.size(), lines.data(), nullptr);
    std::for_each(lines.begin(), lines.end(),  [] (GLchar *line) { free(line); });
    std::cout << "loaded shader source " << path << std::endl;
}

void Shader::compile() {
    glCompileShader(_handle);

    GLint isCompiled = 0;
    glGetShaderiv(_handle, GL_COMPILE_STATUS, &isCompiled);
    if (!isCompiled) {
        GLint maxLength = 0;
        glGetShaderiv(_handle, GL_INFO_LOG_LENGTH, &maxLength);
        std::basic_string<GLchar> errorLog;
        errorLog.resize(maxLength);
        glGetShaderInfoLog(_handle, maxLength, nullptr, errorLog.data());
        throw std::runtime_error { "shader compile log: " + errorLog };
    }
    std::cout << "successfully compiled shader handle=" << _handle << std::endl;
}


Program::Program(const SharedShader &vs, const SharedShader &fs)
    : _handle { glCreateProgram() },
      _vs { vs }, _fs { fs } {
    if (_handle == 0) {
        throw std::runtime_error { "could not create shader program" };
    }

    glAttachShader(_handle, _vs->_handle);
    glAttachShader(_handle, _fs->_handle);
    link();
}

Program::~Program() noexcept {
    glDeleteProgram(_handle);
}

void Program::use() noexcept {
    glUseProgram(_handle);
}

void Program::link() {
    glLinkProgram(_handle);

    GLint isLinked = 0;
    glGetProgramiv(_handle, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        glGetProgramiv(_handle, GL_INFO_LOG_LENGTH, &maxLength);
        std::basic_string<GLchar> errorLog;
        errorLog.resize(maxLength);
        glGetProgramInfoLog(_handle, maxLength, &maxLength, errorLog.data());
        throw std::runtime_error { "shader link log: " + errorLog };
    }
    std::cout << "linked shader program" << std::endl;
}

GLuint Program::getLocation(const std::string &name) {
    const GLint location = glGetUniformLocation(_handle, name.c_str());
    if (location == -1 || glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not get uniform location " + name };
    }
    return location;
}

void Program::setUniform(GLuint location, GLuint value) {
    glProgramUniform1ui(_handle, location, value);
}

void Program::setUniform(GLuint location, GLfloat value) {
    glProgramUniform1f(_handle, location, value);
}

void Program::setUniform(GLuint location, const vec3 &vector) {
    glProgramUniform3fv(_handle, location, 1, glm::value_ptr(vector));
}

void Program::setUniform(GLuint location, const mat4 &matrix) {
    glProgramUniformMatrix4fv(_handle, location, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Program::setUniform(const std::string &name, bool value) {
    setUniform(name, static_cast<GLuint>(value));
}

void Program::setUniform(const std::string &name, GLuint value) {
    setUniform(getLocation(name), value);
}

void Program::setUniform(const std::string &name, GLfloat value) {
    setUniform(getLocation(name), value);
}

void Program::setUniform(const std::string &name, const vec3 &vector) {
    setUniform(getLocation(name), vector);
}

void Program::setUniform(const std::string &name, const mat4 &matrix) {
    setUniform(getLocation(name), matrix);
}

void Program::setUniform(const std::string &name, const SharedTexture &texture) {
    if (texture == nullptr) {
        throw std::invalid_argument { "invalid texture" };
    }
    const GLuint location = getLocation(name);
    const GLuint textureUnit = 0;  // TODO(bkuolt): add support for more than one texture

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    texture->bind();
    glProgramUniform1i(_handle, location, textureUnit);
}

}  // namespace bgl
