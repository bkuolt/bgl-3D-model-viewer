#pragma once

/**
 * @file scene.hpp
 * @brief Lighting and scene environment representations.
 */

#include "gfx/math.hpp"

namespace bgl {

/**
 * @brief Directional light source definition.
 */
struct DirectionalLight {
    vec3 direction{0.0f, -1.0f, 0.0f};
    vec3 diffuse{1.0f};
    vec3 ambient{0.2f};
};

}  // namespace bgl
