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

/**
 * @brief A simple scene grap approach.
 */
class Scene {
 public:
    Scene() = default;
    virtual ~Scene() noexcept = default;

    /**
     * @brief Renders this scene.
     * 
     */
    virtual void render();

    /**
     * @brief Adds a model to this scene.
     * 
     */
    void add(const std::shared_ptr<Model>&);

    /**
     * @brief  Adds a light source to this scene.
     * 
     */
    void add(const std::shared_ptr<Light>&);

    /**
     * @brief Removes a model from this scene
     * 
     */
    void remove(const std::shared_ptr<Model>&);

    /**
     * @brief Removes a light source from this scene.
     * 
     */
    void remove(const std::shared_ptr<Light>&);

    /**
     * @brief Sets the camera of this scene.
     */
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
