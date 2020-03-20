// Copyright 2020 Bastian Kuolt
#ifndef GFX_SHADER_HPP_
#define GFX_SHADER_HPP_

#include "gl.hpp"

#include <filesystem>
#include <memory>
#include <string>


namespace bgl {

class Shader {
 public:
    const GLuint _handle;
    const GLuint _type;

    explicit Shader(GLenum type, const std::filesystem::path & path);
    Shader(const Shader&) = delete;
    virtual ~Shader() noexcept;

    Shader& operator=(const Shader&) = delete;

 private:
    void load(const std::filesystem::path & path);
    void compile();
};

using SharedShader = std::shared_ptr<Shader>;

inline SharedShader LoadShader(GLenum type, const std::filesystem::path &path) {
    return std::make_shared<Shader>(type, path);
}

class Program {
 public:
	Program(const SharedShader &vs, const SharedShader &fs_shader);
	Program(const Program&) = delete;
	virtual ~Program() noexcept;

	Program& operator=(const Program&) = delete;

	void setUniform(const std::string &name, GLfloat value);
	void setUniform(const std::string &name, const vec3 &vector);
	void setUniform(const std::string &name, const mat4 &matrix);

	void setUniform(GLuint location, GLfloat value);
	void setUniform(GLuint location, const vec3 &vector);
	void setUniform(GLuint location, const mat4 &matrix);

	const GLuint _handle;

 private:
    void link();
	GLuint getLocation(const std::string &name);

    SharedShader _vs;
    SharedShader _fs;
};

using SharedProgram = std::shared_ptr<Program>;

}  // namespace bgl

#endif  // GFX_SHADER_HPP_
