// Copyright 2020 Bastian Kuolt
#include "shader.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>


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
        // std::cout << std::quoted(line) << std::endl;
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
    const GLuint location = glGetUniformLocation(_handle, name.c_str());
    if (location == 0 || glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "could not get uniform location" };
    }
    return location;
}

void Program::setUniform(GLuint location, GLfloat value) {
    glProgramUniform1f(_handle, location, value);
}

void Program::setUniform(GLuint location, const vec3 &vector) {
    glProgramUniform3fv(_handle, location, 1, glm::value_ptr(vector));
}

void Program::setUniform(GLuint location, const mat4 &matrix) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
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


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
#if 0
// #pragma include(“whatsits.h”)
struct Light {};

struct DirectionalLight : public Light {
    vec3 direction;
    vec3 color;
};


void SetUniform(const ShaderProgram &program, const Light &light) {
    program->setUniform("Light.direction"), 1, myFloats);
    program->setUniform("Light.color"), 1, &(myFloats[4]));
}
#endif  // 0
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


}  // namespace bgl
