// Copyright 2021 Bastian Kuolt
#include "model.hpp"
#include "box.hpp"
#include "gfx.hpp"  //  model

#include <algorithm>
#include <iostream>
#include <list>
#include <string>

#include <QImage>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "gfx.hpp"


namespace bgl {

namespace {

inline QVector3D to_qt(const glm::vec3 &v) noexcept {
    return { v.x, v.y, v.z };
}

/*********************************************************
 *                      Lighting Code                    *
 *********************************************************/
struct DirectionalLight {
    vec3 direction;
    vec3 diffuse;
    vec3 ambient;
};

void setupLight(QOpenGLShaderProgram &program, const DirectionalLight &light) {
    program.setUniformValue("light.direction", to_qt(light.direction));
    program.setUniformValue("light.diffuse", to_qt(light.diffuse));
    program.setUniformValue("light.ambient", to_qt(light.ambient));
}

void setupTexture(QOpenGLShaderProgram &program, QOpenGLTexture &texture,
                  const std::string &name, GLuint textureUnit = 0) {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    texture.bind();
    program.setUniformValue(name.c_str(), textureUnit);
}

void setupMaterial(QOpenGLShaderProgram &program, const Material &material) {
    program.setUniformValue("material.ambient", to_qt(material.ambient));
    program.setUniformValue("material.diffuse", to_qt(material.diffuse));
    program.setUniformValue("material.specular", to_qt(material.specular));
    program.setUniformValue("material.shininess", material.shininess);

    /**
     * @note There is currently only support for diffuse texture maps. 
     */
    const GLuint isTextured { material.textures.diffuse != nullptr };
    program.setUniformValue("material.isTextured", isTextured);
    if (isTextured) {
        setupTexture(program, *material.textures.diffuse, "material.texture");
    }
}

void setupLighting(QOpenGLShaderProgram &program) {
    static DirectionalLight light {
        .direction = vec3 { -1.0, -1.0, -1.0 },
        .diffuse = vec3 { 0.0, 1.0, 1.0 },
        .ambient = vec3 { 0.2f, 0.2f, 0.2f }
    };

    setupLight(program, light);
}

}  // anonmous namespace


void Model::render(const mat4 &MVP) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    _program->bind();

    setupLighting(*_program);
    const QMatrix4x4 matrix { glm::value_ptr(MVP) };
    _program->setUniformValue("MVP", matrix.transposed());

    /**
     * @brief Render a mesh for each material as there is is one VBO per material
     * @details http://assimp.sourceforge.net/lib_html/materials.html
     */
    for (auto i = 0u; i < _meshes.size(); ++i) {
        if (_meshes[i]._materialIndex.has_value()) {
            const unsigned int material_index { _meshes[i]._materialIndex.value() };
            setupMaterial(*_program, _materials[material_index]);
        }
        _meshes[i].render(GL_TRIANGLES);
    }
}

void Model::resize(const vec3 &dimensions) {
    _boundingBox.resize(dimensions);
}

const BoundingBox& Model::getBoundingBox() const {
    return _boundingBox;
}

}  // namespace bgl
