#ifndef GFX_MATERIAL_HPP_
#define GFX_MATERIAL_HPP_

#include <glm/glm.hpp>
#include <memory>

class QOpenGLTexture;

namespace bgl  {

using namespace glm;

struct Material {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    vec3 emissive;
	float shininess;

    struct {
        std::shared_ptr<QOpenGLTexture> diffuse;
        std::shared_ptr<QOpenGLTexture> ambient;
        std::shared_ptr<QOpenGLTexture> specular;
        std::shared_ptr<QOpenGLTexture> emissive;
    } textures;
};

}  // namespace bgl

#endif  // GFX_MATERIAL_HPP_
