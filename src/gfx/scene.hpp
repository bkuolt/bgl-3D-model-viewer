// Copyright 2021 Bastian Kuolt
#ifndef GFX_SCENE_HPP_
#define GFX_SCENE_HPP_

#include <memory>  // std::shared_ptr
#include <set>

#include "gl.hpp"
#include "model.hpp"
#include "camera.hpp"

#include "light.hpp"


namespace bgl {


class Scene {
 public:
    Scene() = default;
    virtual ~Scene() noexcept = default;

    /**
     * @brief Renders the scene.
     * 
     */
    virtual void render();

    void add(const std::shared_ptr<Model>&);
    void add(const std::shared_ptr<DirectionalLight>&);

    void remove(const std::shared_ptr<Model>&);
    void remove(const std::shared_ptr<DirectionalLight>&);

    void setCamera(const Camera&);

    Camera& getCamera() noexcept;
    const Camera& getCamera() const noexcept;

 private:
	Camera _camera;
    std::set<std::shared_ptr<Model>> _models;
    std::set<std::shared_ptr<Light>> _lights;
};

}  // namesapce scene

#endif  // GFX_SCENE_HPP_
