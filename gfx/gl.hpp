// Copyright 2020 Bastian Kuolt
#ifndef GFX_GL_HPP_
#define GFX_GL_HPP_

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/gtc/type_ptr.hpp>  // glm::value_ptr()

namespace bgl {

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using mat4 = glm::mat4;

}  // namespace bgl

#endif  // GFX_GL_HPP_