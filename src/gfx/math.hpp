#pragma once

/**
 * @file math.hpp
 * @brief Mathematical types and utilities using GLM.
 */

#include <GL/gl.h>
#include <iomanip>
#include <ostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/transform.hpp>

namespace bgl {

/* Vector type aliases */
using ivec2 = glm::tvec2<GLint>;
using uvec2 = glm::tvec2<GLuint>;
using vec2 = glm::tvec2<GLfloat>;
using vec3 = glm::tvec3<GLfloat>;

template <typename T>
using tvec2 = glm::tvec2<T>;

template <typename T>
using tvec3 = glm::tvec3<T>;

/* Matrix type aliases */
using mat3 = glm::mat3;
using mat4 = glm::mat4;

template <typename T>
std::ostream& operator<<(std::ostream& os, const tvec2<T>& v) {
    const std::streamsize previous_precision{os.precision(2)};
    os << "(" << std::fixed << v.x << " | " << std::fixed << v.y << ")";
    os.precision(previous_precision);
    return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, const tvec3<T>& v) {
    const std::streamsize previous_precision{os.precision(2)};
    os << "(" << std::fixed << v.x << " | " << std::fixed << v.y << " | " << std::fixed << v.z
       << ")";
    os.precision(previous_precision);
    return os;
}

}  // namespace bgl
