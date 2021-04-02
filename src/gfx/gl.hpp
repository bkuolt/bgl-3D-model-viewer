#ifndef GFX_GL_GL_HPP_
#define GFX_GL_GL_HPP_

// OpenGL support
#define GLEW_STATIC

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <QOpenGLVertexArrayObject>

// GLM support
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>  // glm::lookAt(), glm::ortho()
#include <glm/gtx/rotate_vector.hpp>     // glm::rotate
#include <glm/gtx/transform.hpp>         // glm::rotate(), glm::scale()
#include <glm/gtc/type_ptr.hpp>          // glm::value_ptr()

#include <iomanip>
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
