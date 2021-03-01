// Copyright 2021 Bastian Kuolt
#include "model.hpp"
#include "box.hpp"

#include <assimp/cimport.h>      // aiPropertyStore
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/Importer.hpp>   // Model loader
#include <assimp/material.h>
#include <assimp/scene.h>        // Output data structure

#include <algorithm>
#include <iostream>
#include <list>
#include <string>

#include <QImage>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

#include "gfx.hpp"  // details;


namespace {
using namespace bgl;

namespace details {

inline QVector3D to_qt(const glm::vec3 &v) noexcept {
    return { v.x, v.y, v.z };
}

inline bool is_textured(const aiMesh &mesh) noexcept {
    return mesh.mTextureCoords[0] != nullptr;
}

inline bool has_material(const aiMesh &mesh) noexcept {
    return mesh.mMaterialIndex != 0;
}

/*********************************************************
 *                     OpenGL Code                       *
 *********************************************************/
QOpenGLBuffer create_vbo(QOpenGLBuffer &vbo, const aiMesh &mesh) {
    vbo.bind();
    vbo.allocate(sizeof(Vertex) * mesh.mNumVertices);
    auto buffer { reinterpret_cast<Vertex*>(vbo.map(QOpenGLBuffer::ReadWrite)) };
    if (buffer == nullptr) {
        vbo.release();
        throw std::runtime_error { "could not map VBO" };
    }

    for (auto i = 0u; i < mesh.mNumVertices; ++i) {
        buffer[i].normal = vec3 { mesh.mNormals[i].x, mesh.mNormals[i].y, mesh.mNormals[i].z };
        buffer[i].position = vec3 { mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z };
    }

    if (is_textured(mesh)) {
        if (mesh.mNumUVComponents[0] != 2) {
            vbo.unmap();
            vbo.release();
            throw std::runtime_error { "only one texture channel supported" };
        }
        for (unsigned int i = 0; i < mesh.mNumVertices; ++i) {
            buffer[i].texcoords = vec2 { mesh.mTextureCoords[0][i].x, 1.0 - mesh.mTextureCoords[0][i].y };
        }

    }

    if (!vbo.unmap()) {
        vbo.release();
        throw std::runtime_error { "could not unmap VBO" };
    }
    vbo.release();
    return vbo;
}

void create_ibo(QOpenGLBuffer &ibo, const aiMesh &mesh) {
    ibo.bind();
    ibo.allocate(sizeof(GLuint) * mesh.mNumFaces * 3);
    GLuint * buffer { reinterpret_cast<GLuint*>(ibo.map(QOpenGLBuffer::WriteOnly)) };
    if (buffer == nullptr) {
        throw std::runtime_error { "could not map IBO" };
    }

    for (auto i = 0u; i < mesh.mNumFaces; ++i) {
        assert(mesh.mFaces[i].mNumIndices == 3);
            std::copy_n(mesh.mFaces[i].mIndices, 3, buffer);
            buffer += 3;
    }

    if (!ibo.unmap()) {
        throw std::runtime_error { "could not unmap IBO" };
    }
    ibo.release();
}

// vao must be bound!
void set_va_attribute(GLuint location, GLsizei size, GLenum type, GLsizei stride, GLsizei offset) {
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(location, size, type, GL_FALSE, stride, reinterpret_cast<void*>(offset));

    int error = glGetError();
    if (error != GL_NO_ERROR) {
        std::cout << "glVertexAttribPointer() failed for location " << location << " due to "
                  << gluErrorString(error) << std::endl;
        throw std::runtime_error { "glVertexAttribPointer() failed" };
    }
}

// program must be bound!!!!
void create_vao(QOpenGLVertexArrayObject &vao, QOpenGLBuffer &vbo, QOpenGLShaderProgram &program) {
    program.bind();
    vao.bind();
    vbo.bind();
    const auto stride { sizeof(Vertex) };
    set_va_attribute(program.attributeLocation("position"), 3, GL_FLOAT, stride, offsetof(Vertex, position));
    set_va_attribute(program.attributeLocation("normal"), 3, GL_FLOAT, stride, offsetof(Vertex, normal));
    set_va_attribute(program.attributeLocation("texcoords"), 2, GL_FLOAT, stride, offsetof(Vertex, texcoords));
    vao.release();
    vbo.release();
    program.release();
}

/*********************************************************
 *                     Assimp Mesh Code                  *
 *********************************************************/
const aiScene* importScene(const std::filesystem::path &path) {
    if (!std::filesystem::exists(path)) {
        std::ostringstream oss;
        oss << "the file " << std::quoted(path.string()) << " does not exist";
        throw std::runtime_error { oss.str() };
    }

    aiPropertyStore* props = aiCreatePropertyStore();
    if (props == nullptr) {
        throw std::runtime_error { aiGetErrorString() };
    }

    aiSetImportPropertyInteger(props, AI_CONFIG_PP_PTV_NORMALIZE, 1);
    const aiScene * scene { aiImportFileExWithProperties(path.string().c_str(),
        aiProcess_Triangulate            |
        aiProcess_GenSmoothNormals |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices,
        nullptr,
        props)
    };

    aiReleasePropertyStore(props);
    return scene ? scene
                 : throw std::runtime_error { aiGetErrorString() };
}


void load_meshes(std::vector<Mesh> &meshes, const aiScene &scene, QOpenGLShaderProgram &program) {
    if (scene.mNumMeshes == 0) {
        throw std::runtime_error { "empty model" };
    }

    meshes = std::vector<Mesh>(scene.mNumMeshes);
    std::cout << "loading " << meshes.size()  << " meshes" << std::endl;
    
    for (auto i = 0u; i < meshes.size(); ++i) {
        const aiMesh &ai_mesh { *scene.mMeshes[i] };
        create_vbo(meshes[i]._vbo, ai_mesh);
        create_ibo(meshes[i]._ibo, ai_mesh);
        create_vao(meshes[i]._vao, meshes[i]._vbo, program);

        if (has_material(ai_mesh)) {
            meshes[i]._materialIndex = ai_mesh.mMaterialIndex;
        }
    }
}

BoundingBox calculate_bounding_box(const aiScene &scene) noexcept {
    BoundingBox boundingBox;
    for (auto i = 0u; i < scene.mNumMeshes; ++i) {
        const aiMesh &mesh { *scene.mMeshes[i] };
        for (auto vertex_index = 0u; vertex_index < mesh.mNumVertices; ++vertex_index) {
            for (auto c = 0u; c < 3; ++c) {
                boundingBox._bounds[c].min = std::min(boundingBox._bounds[c].min, mesh.mVertices[vertex_index][i]);
                boundingBox._bounds[c].max = std::max(boundingBox._bounds[c].max, mesh.mVertices[vertex_index][i]);
            }
        }
    }
    return boundingBox;
}

/*********************************************************
 *                   Assimp Material Code                *
 *********************************************************/
vec3 get_color(const aiMaterial &material,
                      const char *pKey, unsigned int type, unsigned int idx) {
    aiColor3D color;
    material.Get(pKey, type, idx, color);
    return { color.r, color.g, color.b };
}

float get_shininess(const aiMaterial &material) {
    // SHININESS
    return 0;
}

std::shared_ptr<QOpenGLTexture> get_texture(const aiMaterial &material, aiTextureType type, const std::filesystem::path &base_path) {
    const unsigned int texture_count { material.GetTextureCount(type) };
    if (texture_count >= 1) {
        aiString path;
        material.GetTexture(type, 0, &path);
        QImage image { (base_path / path.data).string().c_str() };

        std::cout << "loading " << (base_path / path.data).string().c_str() << std::endl;
        if (texture_count > 1) {
            std::cout << "warning: found more textures than expected" << std::endl;
        }

        return std::make_shared<QOpenGLTexture>(image);
    }

    return {};
}

Material load_material(const aiMaterial &material, const std::filesystem::path &base_path) {
    return {
        .diffuse = details::get_color(material,  AI_MATKEY_COLOR_DIFFUSE),
        .ambient = details::get_color(material,  AI_MATKEY_COLOR_AMBIENT),
        .specular = details::get_color(material, AI_MATKEY_COLOR_SPECULAR),
        .emissive = details::get_color(material, AI_MATKEY_COLOR_EMISSIVE),
        .shininess = get_shininess(material),
        .textures {
            .diffuse = details::get_texture(material, aiTextureType_DIFFUSE, base_path),
            .ambient = details::get_texture(material, aiTextureType_AMBIENT, base_path),
            .specular = details::get_texture(material, aiTextureType_SPECULAR, base_path),
            .emissive = details::get_texture(material, aiTextureType_EMISSIVE, base_path)
        }
    };
}

std::vector<Material> load_materials(const aiScene &scene, const std::filesystem::path &base_path) {
    std::cout << "loading " << scene.mNumMaterials << " materials" << std::endl;
    std::vector<Material> materials;
    for (auto i = 0u; i < scene.mNumMaterials; ++i) {
        materials.push_back(details::load_material(*scene.mMaterials[i], base_path) );
    }
    return materials;
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

}  // namespace details


void setupMaterial(QOpenGLShaderProgram &program, const Material &material) {
    program.setUniformValue("material.ambient", details::to_qt(material.ambient));
    program.setUniformValue("material.diffuse", details::to_qt(material.diffuse));
    program.setUniformValue("material.specular", details::to_qt(material.specular));
    program.setUniformValue("material.shininess", material.shininess);

    /**
     * @note There is currently only support for diffuse texture maps. 
     */
    const GLuint isTextured { material.textures.diffuse != nullptr };
    program.setUniformValue("material.isTextured", isTextured);
    if (isTextured) {
        details::setupTexture(program, *material.textures.diffuse, "material.texture");
    }
}

void setupLighting(QOpenGLShaderProgram &program) {
    static details::DirectionalLight light {
        .direction = vec3 { -1.0, -1.0, -1.0 },
        .diffuse = vec3 { 0.0, 1.0, 1.0 },
        .ambient = vec3 { 0.2f, 0.2f, 0.2f }
    };

    details::setupLight(program, light);
}

}  // anonmous namespace


namespace bgl {


void Model::resize(const vec3 &dimensions) {
    _boundingBox.resize(dimensions);
}

const BoundingBox& Model::getBoundingBox() const {
    return _boundingBox;
}

/* ------------------------------ Mesh --------------------------------- */

Model::Model(const std::filesystem::path &path) {
    const aiScene &scene { *details::importScene(path) };

    _program = LoadProgram("./assets/shaders/main.vs", "./assets/shaders/main.fs");
    details::load_meshes(_meshes, scene, *_program);

    _materials = details::load_materials(scene, path.parent_path());
    _boundingBox = details::calculate_bounding_box(scene);
}

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

}  // namespace bgl
