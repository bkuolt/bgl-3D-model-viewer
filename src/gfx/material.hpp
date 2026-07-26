#pragma once

/**
 * @file material.hpp
 * @brief Material and texture properties for rendered meshes.
 */

#include <memory>

#include "gfx/math.hpp"

class QOpenGLTexture;

namespace bgl {

struct Material {
    vec3 diffuse{0.0f};
    vec3 ambient{0.0f};
    vec3 specular{0.0f};
    vec3 emissive{0.0f};
    float shininess{0.0f};

    struct {
        std::shared_ptr<QOpenGLTexture> diffuse;
        std::shared_ptr<QOpenGLTexture> ambient;
        std::shared_ptr<QOpenGLTexture> specular;
        std::shared_ptr<QOpenGLTexture> emissive;
    } textures;
};

}  // namespace bgl
