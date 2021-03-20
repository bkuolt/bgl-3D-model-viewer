#ifndef GFX_LIGHT_HPP
#define GFX_LIGHT_HPP

#include "gl.hpp"

namespace bgl {

struct Light {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};

struct DirectionalLight : public Light {
    vec3 direction;
};

struct PointLight : public Light {
    vec3 position;
    struct {
        float constant;
        float linear;
        float exp;
    } attenuation;
};

struct Spotlight : public Light{
    // TODO
};

}  // namespace bgl

#endif  // GFX_LIGHT_HPP
