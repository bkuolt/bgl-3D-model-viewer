
#ifndef GFX_MODEL_HPP_
#define GFX_MODEL_HPP_

#include <filesystem>
#include <memory>  // std::shared_ptr
#include <vector>

#include "gl.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "bounding_box.hpp"

#include <QOpenGLShaderProgram>


namespace bgl {

struct DirectionalLight {
    vec3 direction;
    vec3 diffuse;
    vec3 ambient;
};  // TODO: move to scene.hpp
