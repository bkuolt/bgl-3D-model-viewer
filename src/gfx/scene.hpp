// Copyright 2021 Bastian Kuolt
#ifndef GFX_SCENE_HPP_
#define GFX_SCENE_HPP_

#include "gl.hpp"
#include "model.hpp"
#include "camera.hpp"


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


class Scene {
 public:
    Scene() = default;
    virtual ~Scene() noexcept = default;

    virtual void render();

    void add(const std::shared_ptr<Model> &model);
    void add(const shared_ptr<DirectionalLight> &light);
    void setCamera(const Camera &camera);

 private:
	Camera camera;
    std::vector<std::shared_ptr<Model>> _models;
    std::vector<std::shared_ptr<Light>> _lights;
};

}  // namesapce scene

#endif  // GFX_SCENE_HPP_
