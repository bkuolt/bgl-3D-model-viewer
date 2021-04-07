
#ifndef GFX_MODEL_HPP_
#define GFX_MODEL_HPP_

#include <filesystem>
#include <memory>  // std::shared_ptr
#include <vector>

#include "gl.hpp"
#include "mesh.hpp"
#include "material.hpp"
#include "bounding_box.hpp"
#include "scene.hpp"

#include <QOpenGLShaderProgram>  // NOLINT


namespace bgl {

/**
 * @brief An OpenGL renderable mesh.
 */
class Model {
 public:
	Model() = default;
	Model(Model&&) = default;
	Model& operator=(Model&&) = default;

	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;

	virtual ~Model() noexcept = default;

	virtual void render(const mat4 &MVP);
	virtual void render(const mat4 &MVP, const DirectionalLight &light);

	void resize(const vec3 &dimensions);
	const BoundingBox& getBoundingBox() const;

	void setMaterials(std::vector<Material> materials) {
		_materials = materials;
	}

	void setProgram(std::shared_ptr<QOpenGLShaderProgram> program) {
		_program = program;
	}

	void setBoundingBox(const BoundingBox &boundingBox) {
		_boundingBox = boundingBox;
	}

	const std::vector<Mesh>& getMeshes() const noexcept {
		return _meshes;
	}

	std::vector<Mesh>& getMeshes() noexcept {
		return _meshes;
	}

	const std::shared_ptr<QOpenGLShaderProgram> getProgram() const noexcept {
		return _program;
	}

 protected:
	std::vector<Mesh> _meshes;
	std::vector<Material> _materials;

	std::shared_ptr<QOpenGLShaderProgram> _program;
	BoundingBox _boundingBox;
};

/**
 * @brief Loads a 3D model file.
 */
std::shared_ptr<Model> LoadModel(const std::filesystem::path &path);  // TODO defined in importer.cpp

}  // namespace bgl

#endif  // GFX_MODEL_HPP_
