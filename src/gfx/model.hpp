// Copyright 2021 Bastian Kuolt
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

class Model {
 public:
	Model() = default;
	virtual ~Model() noexcept = default;

	virtual void render(const mat4 &MVP);

	void resize(const vec3 &dimensions);
	const BoundingBox& getBoundingBox() const;

 protected: public:
	std::vector<Mesh> _meshes;
	std::vector<Material> _materials;

	std::shared_ptr<QOpenGLShaderProgram> _program;
	BoundingBox _boundingBox;
};

/**
 * @brief Loads a 3D model file.
 */
std::shared_ptr<Model> LoadModel(const std::filesystem::path &path);  // defined in importer.cpp

}  // namespace bgl

#endif  // GFX_MODEL_HPP_
