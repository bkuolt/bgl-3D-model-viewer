#pragma once

/**
 * @file gl.hpp
 * @brief OpenGL header includes and configuration for the bgl rendering engine.
 */

#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
// clang-format on

#include "gfx/math.hpp"
