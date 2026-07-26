#pragma once

/**
 * @file model.hpp
 * @brief Base 3D Model class managing mesh collections, materials, and shaders.
 */

#include <QOpenGLShaderProgram>
#include <filesystem>
#include <memory>
#include <vector>

#include "gfx/bounding_box.hpp"
#include "gfx/gl.hpp"
#include "gfx/material.hpp"
#include "gfx/mesh.hpp"
#include "gfx/scene.hpp"

namespace bgl {

/**
 * @brief High-level renderable 3D model object.
 */
class Model {
  public:
    Model() = default;
    virtual ~Model() noexcept = default;

    Model(Model&&) noexcept = default;
    Model& operator=(Model&&) noexcept = default;

    Model(const Model&) = delete;
    Model& operator=(const Model&) = delete;

    virtual void render(const mat4& MVP);
    virtual void render(const mat4& MVP, const DirectionalLight& light);

    void resize(const vec3& dimensions);
    [[nodiscard]] const BoundingBox& getBoundingBox() const noexcept;

    void setMaterials(std::vector<Material> materials) {
        _materials = std::move(materials);
    }

    void setProgram(std::shared_ptr<QOpenGLShaderProgram> program) {
        _program = std::move(program);
    }

    void setBoundingBox(const BoundingBox& boundingBox) noexcept {
        _boundingBox = boundingBox;
    }

    [[nodiscard]] const std::vector<Mesh>& getMeshes() const noexcept {
        return _meshes;
    }

    [[nodiscard]] std::vector<Mesh>& getMeshes() noexcept {
        return _meshes;
    }

    [[nodiscard]] std::shared_ptr<QOpenGLShaderProgram> getProgram() const noexcept {
        return _program;
    }

  protected:
    std::vector<Mesh> _meshes;
    std::vector<Material> _materials;

    std::shared_ptr<QOpenGLShaderProgram> _program;
    BoundingBox _boundingBox;
};

}  // namespace bgl
