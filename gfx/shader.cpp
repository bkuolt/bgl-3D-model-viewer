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

}  // namespace bgl
