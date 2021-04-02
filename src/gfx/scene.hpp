#ifndef GFX_SCENE_HPP_
#define GFX_SCENE_HPP_

#include "math.hpp"


namespace bgl {

struct DirectionalLight {
    vec3 direction;
    vec3 diffuse;
    vec3 ambient;
};

}  // namepace bgl

#endif  // GFX_SCENE_HPP_
