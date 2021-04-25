/**
 * @file Scene.hpp
 * @brief 
 */
#ifndef SCENE_HPP_
#define SCENE_HPP_

#include <memory>  // std::shared_ptr
#include <filesystem>

#include "OpenGL.hpp"         // TODO
#include "../gfx/camera.hpp"  // TODO

#include <QImage>
#include <QOpenGLTexture>


namespace bgl {

class Model;
class Box;
class Grid;

class Scene {
 public:
	Scene() = default;
	virtual ~Scene() = default;

	virtual void draw();

	void load(const std::filesystem::path &path);
	void add(const std::shared_ptr<Model> &model);
	ArcBall& camera() noexcept;

 private:
	std::shared_ptr<Model> _model;
	std::shared_ptr<Grid> _grid;
	std::shared_ptr<Box> _box;
	ArcBall _camera;
};

}  // namespace bgl

#endif  // SCENE_HPP_
