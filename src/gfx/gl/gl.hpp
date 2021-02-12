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


#include <iomanip>  // std::fixed
#include <ostream>

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

}  // namespace bgl

#endif  // GFX_GL_GL_HPP_
