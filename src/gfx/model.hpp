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

struct DirectionalLight {
    vec3 direction;
    vec3 diffuse;
    vec3 ambient;
};  // TODO: move to scene.hpp


/**
 * @brief 
 * 
 */
class Model {
 public:
	Model() = default;
	virtual ~Model() noexcept = default;

	virtual void render(const mat4 &MVP) {}
	virtual void render(const mat4 &MVP, const DirectionalLight &light);

	void resize(const vec3 &dimensions);
	const BoundingBox& getBoundingBox() const;
	// TODO: non const version

 public:
	std::vector<Mesh> _meshes;
	std::vector<Material> _materials;

	std::shared_ptr<QOpenGLShaderProgram> _program;
	BoundingBox _boundingBox;
};

/**
 * @brief 
 * 
 * @param path 
 * @return std::shared_ptr<Model> 
 */
std::shared_ptr<Model> LoadModel(const std::filesystem::path &path);  // defined in importer.cpp

}  // namespace bgl

#endif  // GFX_MODEL_HPP_
