// Copyright 2020 Bastian Kuolt
#ifndef GFX_GL_GL_HPP_
#define GFX_GL_GL_HPP_

// OpenGL support
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "math.hpp"

#include <QOpenGLVertexArrayObject>

#include <memory>


namespace bgl {

/* ------------------------ Uniform Setter Helper ------------------------ */

template<GLenum Type, GLsizei Count>
struct GLtype_traits_base {
    static const GLenum type { Type };
    static const GLsizei count { Count };
};

template<typename T> struct GLtype_traits {};
template<> struct GLtype_traits<GLuint> : public GLtype_traits_base<GL_UNSIGNED_INT, 1> {};
template<> struct GLtype_traits<GLfloat> : public GLtype_traits_base<GL_FLOAT, 1> {};
template<> struct GLtype_traits<vec2> : public GLtype_traits_base<GL_FLOAT, 2> {};
template<> struct GLtype_traits<vec3> : public GLtype_traits_base<GL_FLOAT, 3> {};

// TODO(bkuolt): remove
class VertexArrayObject : public QOpenGLVertexArrayObject {
 public:
    void draw(GLenum mode, int count) {
        bind();
        glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error { "glDrawElements() failed" };
        }
    }

    template<typename T>
    void setAttribute(GLuint location, GLsizei stride, GLsizei offset) {
        bind();
        setAttribute_impl(location,
                    GLtype_traits<T>::type, GLtype_traits<T>::count,
                    stride, offset);
    }

 private:
    void setAttribute_impl(GLuint location, GLenum type, GLsizei size, GLsizei stride, GLsizei offset) {
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, size, type, GL_FALSE, stride, reinterpret_cast<void*>(offset));
        if (glGetError() != GL_NO_ERROR) {
            throw std::runtime_error { "glVertexAttribPointer() failed" };
        }
    }

};

}  // namespace bgl

#endif  // GFX_GL_GL_HPP_
