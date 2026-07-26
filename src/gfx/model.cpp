#include "gfx/model.hpp"

#include <QImage>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QVector3D>
#include <algorithm>
#include <iostream>
#include <string>

#include "gfx/box.hpp"
#include "gfx/gfx.hpp"

namespace bgl {

namespace {

inline QVector3D to_qt(const vec3& v) noexcept {
    return {v.x, v.y, v.z};
}

void setupLight(QOpenGLShaderProgram& program, const DirectionalLight& light) {
    program.setUniformValue("light.direction", to_qt(light.direction));
    program.setUniformValue("light.diffuse", to_qt(light.diffuse));
    program.setUniformValue("light.ambient", to_qt(light.ambient));
}

void setupTexture(QOpenGLShaderProgram& program, QOpenGLTexture& texture, const std::string& name,
                  GLuint textureUnit = 0) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    texture.bind();
    program.setUniformValue(name.c_str(), textureUnit);
}

void setupMaterial(QOpenGLShaderProgram& program, const Material& material) {
    program.setUniformValue("material.ambient", to_qt(material.ambient));
    program.setUniformValue("material.diffuse", to_qt(material.diffuse));
    program.setUniformValue("material.specular", to_qt(material.specular));
    program.setUniformValue("material.shininess", material.shininess);

    const auto isTextured = static_cast<GLuint>(material.textures.diffuse != nullptr);
    program.setUniformValue("material.isTextured", isTextured);
    if (isTextured != 0) {
        setupTexture(program, *material.textures.diffuse, "material.texture");
    }
}

}  // anonymous namespace

void Model::render(const mat4& MVP, const DirectionalLight& light) {
    if (!_program) {
        return;
    }

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    _program->bind();

    setupLight(*_program, light);

    const QMatrix4x4 matrix{glm::value_ptr(MVP)};
    _program->setUniformValue("MVP", matrix.transposed());

    for (std::size_t i = 0; i < _meshes.size(); ++i) {
        if (_meshes[i]._materialIndex.has_value()) {
            const unsigned int material_index{_meshes[i]._materialIndex.value()};
            if (material_index < _materials.size()) {
                setupMaterial(*_program, _materials[material_index]);
            }
        }
        _meshes[i].render(GL_TRIANGLES);
    }
}

void Model::render(const mat4& MVP) {
    if (!_program) {
        return;
    }

    _program->bind();
    const QMatrix4x4 matrix{glm::value_ptr(MVP)};
    _program->setUniformValue("MVP", matrix.transposed());

    for (auto& mesh : _meshes) {
        mesh.render(GL_TRIANGLES);
    }
}

void Model::resize(const vec3& dimensions) {
    _boundingBox.resize(dimensions);
}

const BoundingBox& Model::getBoundingBox() const noexcept {
    return _boundingBox;
}

}  // namespace bgl
