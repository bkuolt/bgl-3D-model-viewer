// Copyright 2020 Bastian Kuolt
#ifndef GFX_GL_HPP_
#define GFX_GL_HPP_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>          // glm::value_ptr()
#include <glm/gtc/matrix_transform.hpp>  // glm::lookAt(), glm::ortho()

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>         // glm::rotate()
#include <glm/gtc/matrix_access.hpp>     
#include <glm/gtx/rotate_vector.hpp> // roatate
#include <ostream>
#include <iomanip>  // std::fixed

namespace bgl {

template<typename T> using tvec2 = glm::tvec2<T>;
template<typename T> using tvec3 = glm::tvec3<T>;

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

#endif  // GFX_GL_HPP_
