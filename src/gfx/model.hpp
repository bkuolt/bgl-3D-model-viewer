// Copyright 2021 Bastian Kuolt
#ifndef GFX_MODEL_HPP_
#define GFX_MODEL_HPP_

#include "gl.hpp"

class QOpenGLShaderProgram;

#include <filesystem>
#include <memory>
#include <vector>

#include "mesh.hpp"
#include "material.hpp"
#include "bounding_box.hpp"

#include <QOpenGLShaderProgram>


namespace bgl {

class BasicModel {
 public:
	BasicModel();
	virtual ~BasicModel() noexcept = default;

	virtual void render(const mat4 &MVP) = 0;

	void resize(const vec3 &dimensions);
	const BoundingBox& getBoundingBox() const;

 protected:
	// TODO(bkuolt): virtual void setUniforms(const Mesh &mesh);

	std::vector<Mesh> _meshes;
	std::shared_ptr<QOpenGLShaderProgram> _program;
	std::vector<Material> _materials;
	BoundingBox _boundingBox;
};

class Model : public BasicModel {
 public:
	Model() = default;
	explicit Model(const std::filesystem::path &path);
	virtual ~Model() noexcept = default;

	void render(const mat4 &MVP) override;
};

}  // namespace bgl

#endif  // GFX_MODEL_HPP_
