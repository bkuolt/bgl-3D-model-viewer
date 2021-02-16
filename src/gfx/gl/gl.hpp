// Copyright 2020 Bastian Kuolt
#ifndef GFX_GL_GL_HPP_
#define GFX_GL_GL_HPP_

// OpenGL support
#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

// GLM support
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>  // glm::lookAt(), glm::ortho()
#include <glm/gtx/rotate_vector.hpp>     // glm::rotate
#include <glm/gtx/transform.hpp>         // glm::rotate(), glm::scale()
#include <glm/gtc/type_ptr.hpp>          // glm::value_ptr()

#include <QOpenGLVertexArrayObject>

#include <iomanip>  // std::fixed
#include <ostream>
#include <memory>

namespace bgl {

template<typename T> using tvec2 = glm::tvec2<T>;
template<typename T> using tvec3 = glm::tvec3<T>;

using ivec2 = tvec2<GLint>;
using uvec2 = tvec2<GLuint>;
using vec2 = tvec2<GLfloat>;
using vec3 = tvec3<GLfloat>;

using mat4 = glm::mat4;

template<typename T>
std::ostream& operator<<(std::ostream &os, const tvec2<T> &vector) {
    const std::streamsize previous_precision { os.precision(2) };
    os << "(" << std::fixed << vector.x << " | " << std::fixed << vector.y << ")";
    os.precision(previous_precision);
    return os;
}

template<typename T>
std::ostream& operator<<(std::ostream &os, const tvec3<T> &vector) {
    const std::streamsize previous_precision { os.precision(2) };
    os << "(" << std::fixed << vector.x << " | "
              << std::fixed << vector.y << " | "
              << std::fixed << vector.z << ")";
    os.precision(previous_precision);
    return os;
}

/* ------------------------ Uniform Setter Helper ------------------------ */
namespace details {

template<GLenum Type, GLsizei Count>
struct type_traits_base {
    static const GLenum type { Type };
    static const GLsizei count { Count };
};

template<typename T> struct type_traits {};
template<> struct type_traits<GLuint> : public type_traits_base<GL_UNSIGNED_INT, 1> {};
template<> struct type_traits<GLfloat> : public type_traits_base<GL_FLOAT, 1> {};
template<> struct type_traits<vec2> : public type_traits_base<GL_FLOAT, 2> {};
template<> struct type_traits<vec3> : public type_traits_base<GL_FLOAT, 3> {};

}  // namespace details


void setAttribute(GLuint location, GLenum type, GLsizei size, GLsizei stride, GLsizei offset) {
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, type, GL_FALSE, stride, reinterpret_cast<void*>(offset));
    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error { "glVertexAttribPointer() failed" };
    }
}

template<typename T>
void SetAttribute(const std::shared_ptr<QOpenGLVertexArrayObject> &vao, GLuint location, GLsizei stride, GLsizei offset) {
    vao->bind();
    setAttribute(location,
                 details::type_traits<T>::type, details::type_traits<T>::count,
                 stride, offset);
}

}  // namespace bgl

#endif  // GFX_GL_GL_HPP_
