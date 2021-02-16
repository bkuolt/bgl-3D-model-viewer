// Copyright 2020 Bastian Kuolt
#include "mesh.hpp"
#include "box.hpp"

#include <assimp/cimport.h>      // aiPropertyStore
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/Importer.hpp>   // Model loader
#include <assimp/scene.h>        // Output data structure

#include <algorithm>
#include <iostream>
#include <list>
#include <string>

#include <QImage>
#include <QMatrix4x4>


namespace bgl {

// TODO(bkuolt): move to in gfx.cpp
std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::filesystem::path &vs, const std::filesystem::path &fs) {
    const auto program { std::make_shared<QOpenGLShaderProgram>() };
    program->addShaderFromSourceFile(QOpenGLShader::Vertex, vs.string().c_str());
    program->addShaderFromSourceFile(QOpenGLShader::Fragment, fs.string().c_str());
    program->link();
    return program;
}

namespace {

/**
 * @brief Checks whether a scene is textured.
 * @note A scene is considered textured if it has at least one diffuse map.
 */
bool IsTextured(const aiScene *scene) {
    std::list<std::filesystem::path> textures;
    for (auto i = 0u; i < scene->mNumMaterials; ++i) {
        if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;
            scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            textures.push_back(path.data);
        }
    }

    if (textures.size() > 1) {
        std::cout << "Warning: Found more than one diffuse map, but only one is supported" << std::endl;
    }
    return !textures.empty();
}

enum AttributLocations : GLuint { MVP = 0, Position = 8, Normal, TexCoord, Texture };

std::shared_ptr<QOpenGLTexture> LoadTexture(const std::filesystem::path &base_path, const aiScene *scene) {
    std::cout << scene->mNumMaterials << " materials" << std::endl;

    for (auto i = 0u; i <  scene->mNumMaterials; ++i) {
        if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;
            scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);

            QImage image { (base_path / path.data).string().c_str() };
            return std::make_shared<QOpenGLTexture>(image);
        }
    }

    return {};  // TODO(bkuolt)
}

/* ------------------------ Lighting ----------------------------------- */

void setUpLightning(std::shared_ptr<QOpenGLShaderProgram> program) {
    constexpr GLsizei maxLightNum = 5;
    static constexpr struct Light {
        vec3 direction { -1.0, -1.0, -1.0 };
        vec3 color { 0.0, 1.0, 1.0 };
    } light;

    program->setUniformValue("lights[0].used", true);
    program->setUniformValue("lights[0].direction", light.direction.x, light.direction.y, light.direction.z);
    program->setUniformValue("lights[0].color", light.color.x, light.color.y, light.color.z);

    for (auto i = 1u; i < maxLightNum; ++i) {
        std::ostringstream name;
        name << "lights[" << i << "].used";
        program->setUniformValue(name.str().c_str(), false);
    }

    // TODO(bkuolt): spearate world uniforms and model uniforms
}

/* --------------------------------------------------------------------- */

aiMesh* getMesh(const aiScene *scene) {
    if (scene->mNumMeshes == 0) {
        throw std::runtime_error { "model file does not contain any mesh" };
    } else if (scene->mNumMeshes > 1) {
        std::cout << "Warning: found more than one mesh, but only one is supported" << std::endl;
    }
    return scene->mMeshes[0];
}

const aiScene* importScene(const std::filesystem::path &path) {
    aiPropertyStore* props = aiCreatePropertyStore();
    if (props == nullptr) {
        throw std::runtime_error { aiGetErrorString() };
    }

    aiSetImportPropertyInteger(props, AI_CONFIG_PP_PTV_NORMALIZE, 1);
    auto scene = aiImportFileExWithProperties(path.string().c_str(),
        aiProcess_Triangulate            |
        aiProcess_GenSmoothNormals |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices,
        nullptr,
        props);
    aiReleasePropertyStore(props);

    if (scene == nullptr) {
        throw std::runtime_error { aiGetErrorString() };
    }
    return scene;
}

BoundingBox GetBoundingBox(const aiMesh *mesh) noexcept {
    BoundingBox boundingBox;

    for (auto i = 0u; i < 3; ++i) {
        boundingBox._bounds[i].min = 0;
        boundingBox._bounds[i].max = 0;
    }

    for (auto vertex_index = 0u; vertex_index < mesh->mNumVertices; ++vertex_index) {
        for (auto i = 0u; i < 3; ++i) {
            boundingBox._bounds[i].min = std::min(boundingBox._bounds[i].min, mesh->mVertices[vertex_index][i]);
            boundingBox._bounds[i].max = std::max(boundingBox._bounds[i].max, mesh->mVertices[vertex_index][i]);
        }
    }

    return boundingBox;
}

}  // namespace

/* ------------------------------- Mesh -------------------------------- */

Mesh::Mesh(const std::filesystem::path &path)
    : _path(path) {
    if (!std::filesystem::exists(path)) {
        std::ostringstream oss;
        oss << "the file " << std::quoted(path.string()) << " does not exist";
        throw std::runtime_error { oss.str() };
    }
}

void Mesh::create() {
    const aiScene *scene { importScene(_path) };
    const aiMesh *mesh { getMesh(scene) };
    createVBO(scene);
    createIBO(mesh);
    createVAO();
    createShaderProgram();
 
    _bounding_box = GetBoundingBox(mesh);

    if (IsTextured(scene)) {
        _texture = LoadTexture(_path.parent_path(), scene);
    }
}

void Mesh::createVBO(const aiScene *scene) {
    const aiMesh &mesh = *scene->mMeshes[0];

    _vbo = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);  
    _vbo->create();
    _vbo->bind();
    _vbo->allocate(sizeof(Vertex) * mesh.mNumVertices);

    auto buffer { reinterpret_cast<Vertex*>(_vbo->map(QOpenGLBuffer::ReadWrite)) };
    std::cout << "p= "<< buffer << std::endl;
    std::cout << "232"<< std::endl;

    const bool is_textured { IsTextured(scene) };
    for (auto vertex_index = 0u; vertex_index < mesh.mNumVertices; ++vertex_index) {
        buffer[vertex_index].normal = vec3 { mesh.mNormals[vertex_index].x, mesh.mNormals[vertex_index].y, mesh.mNormals[vertex_index].z };
        buffer[vertex_index].position = vec3 { mesh.mVertices[vertex_index].x, mesh.mVertices[vertex_index].y, mesh.mVertices[vertex_index].z };
    }

    if (is_textured) {
        for (auto i = 0u; i < mesh.mNumVertices; ++i) {
            buffer[i].texcoords = vec2 { mesh.mTextureCoords[0][i].x,
                                         1.0 - mesh.mTextureCoords[0][i].y };
        }
    }

    _vbo->unmap();
    std::cout << "created vbo containing " << _vbo->size() << " vertices" << std::endl;
}

void Mesh::createIBO(const aiMesh *mesh) {
    _ibo = std::make_shared<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
    _ibo->create();
    _ibo->bind();
    _ibo->allocate(sizeof(GLuint) * mesh->mNumFaces * 3);
    
    auto buffer { reinterpret_cast<GLuint*>(_ibo->map(QOpenGLBuffer::WriteOnly)) };
    for (auto i = 0u; i < mesh->mNumFaces; ++i) {
        switch (mesh->mFaces[i].mNumIndices) {
            case 3:
                std::copy_n(mesh->mFaces[i].mIndices, 3, buffer);
                buffer += 3;
                break;
            default:
                throw std::runtime_error { "unexpected data" };
        }
    }
    _ibo->unmap();

    std::cout << "created an IBO containing " << _ibo->size() << " indices" << std::endl;
}

void Mesh::createVAO() {
    _vao = std::make_shared<VertexArrayObject>();
    _vao->create();
    _vao->bind();

    _ibo->bind();
    _vbo->bind();

    const auto stride { sizeof(Vertex) };
    _vao->setAttribute<vec3>(AttributLocations::Position, stride, offsetof(Vertex, position));
    _vao->setAttribute<vec3>(AttributLocations::Normal, stride, offsetof(Vertex, normal));
    _vao->setAttribute<vec2>(AttributLocations::TexCoord, stride, offsetof(Vertex, texcoords));
}

void Mesh::createShaderProgram() {
    _program = LoadProgram("./assets/shaders/main.vs", "./assets/shaders/main.fs");
}

const BoundingBox& Mesh::getBoundingBox() const {
    return _bounding_box;
}

void Mesh::render(const mat4 &_MVP) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    _program->bind();

    setUpLightning(_program);
    // program->setUniform(AttributLocations::MVP, MVP);
    
    QMatrix4x4 matrix(glm::value_ptr(_MVP));
    _program->setUniformValue("MVP", matrix.transposed());

    const GLuint isTextured { _texture != nullptr };
    _program->setUniformValue("isTextured", isTextured);

    if (isTextured) {
        const GLuint textureUnit { 0 };  // TODO(bkuolt): add support for more than one texture

        glActiveTexture(GL_TEXTURE0 + textureUnit);
        _texture->bind();
        _program->setUniformValue("texture", textureUnit);
    }

    _vao->bind();
    _vbo->bind();
    _ibo->bind();

    _vao->draw(GL_TRIANGLES, _ibo->size() / sizeof(GLuint) * 3);
}

}  // namespace bgl
