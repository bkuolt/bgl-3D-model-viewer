// Copyright 2020 Bastian Kuolt
#include "mesh.hpp"

#include <assimp/cimport.h>      // aiPropertyStore
#include <assimp/postprocess.h>  // Post processing flags
#include <assimp/Importer.hpp>   // Model loader
#include <assimp/scene.h>        // Output data structure

#include <iostream>
#include <string>

namespace bgl {

namespace {

struct bounding_box {
    float min;
    float max;
};

glm::tvec3<bounding_box> get_bounds(const aiMesh &mesh) noexcept {
    glm::tvec3<bounding_box> bounds;

    for (auto i = 0u; i < 3; ++i) {
        bounds[i].min = 0;
        bounds[i].max = 0;
    }

    for (auto vertex_index = 0u; vertex_index < mesh.mNumVertices; ++vertex_index) {
        for (auto i = 0u; i < 3; ++i) {
            bounds[i].min = std::min(bounds[i].min, mesh.mVertices[vertex_index][i]);
            bounds[i].max = std::max(bounds[i].max, mesh.mVertices[vertex_index][i]);
        }
    }
    return bounds;
}

void normalize_vertex_positions(const aiMesh &mesh, Vertex *buffer) {
    const glm::tvec3<bounding_box> bounds = get_bounds(mesh);
    const vec3 dimensions {
        bounds.x.max - bounds.x.min,
        bounds.y.max - bounds.y.min,
        bounds.z.max - bounds.z.min,
    };
    const vec3 center {
        bounds.x.max - (dimensions.x / 2),
        bounds.y.max - (dimensions.y / 2),
        bounds.z.max - (dimensions.z / 2)
    };

    std::cout << "dimensions:"
              << " w=" << dimensions.x
              << " h=" << dimensions.y
              << " d=" << dimensions.z
              << std::endl;

    for (auto vertex_index = 0u; vertex_index < mesh.mNumVertices; ++vertex_index) {
        for (auto i = 0u; i < 3; ++i) {
            buffer[vertex_index].position[i] -= center[i];      // recenter
            buffer[vertex_index].position[i] /= dimensions[i];  // normalize
        }
    }
}

SharedVBO createVBO(const aiScene *scene) {
    static_assert(std::is_same<ai_real, float>::value);
    const aiMesh &mesh = *scene->mMeshes[0];

    auto vbo = std::make_shared<VertexBuffer>(mesh.mNumVertices);
    Vertex *buffer = vbo->map();
    const bool is_textured { scene->mNumTextures > 1 };
    for (auto vertex_index = 0u; vertex_index < mesh.mNumVertices; ++vertex_index) {
        buffer[vertex_index].normal = vec3 { mesh.mNormals[vertex_index].x, mesh.mNormals[vertex_index].y, mesh.mNormals[vertex_index].z };
        buffer[vertex_index].position = vec3 { mesh.mVertices[vertex_index].x, mesh.mVertices[vertex_index].y, mesh.mVertices[vertex_index].z };
        if (is_textured) {
            buffer[vertex_index].texcoords = vec2 { mesh.mTextureCoords[0][vertex_index].x,
                                                    mesh.mTextureCoords[0][vertex_index].y };
        }
    }

    // normalize_vertex_positions(mesh, buffer);
    vbo->unmap();

    std::cout << "created a vbo with " << vbo->size() << " vertices" << std::endl;
    return vbo;
}

SharedIBO createIBO(const aiMesh &mesh) {
    const GLsizei size = mesh.mNumFaces * 3;
    auto ibo = std::make_shared<IndexBuffer>(size);

    auto buffer = ibo->map();
    for (auto i = 0u; i < mesh.mNumFaces; ++i) {
        switch (mesh.mFaces[i].mNumIndices) {
            case 3:
                std::copy_n(mesh.mFaces[i].mIndices, 3, buffer);
                buffer += 3;
                break;
            default:
                throw std::runtime_error { "unexpected data" };
        }
    }
    ibo->unmap();

    std::cout << "created an ibo with " << ibo->size() << " indices" << std::endl;
    return ibo;
}


enum class AttributLocations : GLuint { MVP = 0, Position = 1, Normal, TexCoord, Texture };

SharedVAO createVAO(const SharedVBO &vbo, const SharedIBO &ibo) {
    auto vao = std::make_shared<VertexArray>(vbo, ibo);
    const auto stride = sizeof(Vertex);
    vao->bind();
    SetAttribute<vec3>(vao, (GLuint) AttributLocations::Position, stride, offsetof(Vertex, position));
    SetAttribute<vec3>(vao, (GLuint) AttributLocations::Normal, stride, offsetof(Vertex, normal));
    SetAttribute<vec2>(vao, (GLuint) AttributLocations::TexCoord, stride, offsetof(Vertex, texcoords));
    vao->unbind();
    return vao;
}

SharedTexture loadTexture(const std::filesystem::path &base_path, const aiScene *scene) {
    std::cout << scene->mNumMaterials << " materials" << std::endl;

    for (auto i = 0u; i <  scene->mNumMaterials; ++i) {
        if (scene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
            aiString path;
            scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            return LoadTexture(base_path / path.data);
        }
    }

    return {};
}

/* --------------------------------------------------------------------- */

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

}  // namespace



/* ------------------------------- Mesh -------------------------------- */

Mesh::Mesh(const std::filesystem::path &path) {
    if (!std::filesystem::exists(path)) {
        std::ostringstream oss;
        oss << "The file " << std::quoted(path.string()) << " does not exist";
        throw std::runtime_error { oss.str() };
    }

    const auto scene = importScene(path);
    const aiMesh &mesh = *scene->mMeshes[0];  /// @note currently there is only support for one mesh
    _vbo = createVBO(scene);
    _ibo = createIBO(mesh);
    _vao = createVAO(_vbo, _ibo);

    _texture = loadTexture(path.parent_path(), scene);

    const auto vs = LoadShader(GL_VERTEX_SHADER, "./assets/main.vs");
    const auto fs = LoadShader(GL_FRAGMENT_SHADER, "./assets/main.fs");
    _program = std::make_shared<Program>(vs, fs);
}

static const struct Light {
    vec3 direction { -1.0, -1.0, -1.0 };
    vec3 color { 0.5, 0.0, 1.0 };
} light;

void Mesh::render(const mat4 &MVP) {
    glUseProgram(_program->_handle);

    if (_texture) {
        _program->setUniform((GLuint) AttributLocations::Texture, _texture);
    }

    _program->setUniform((GLuint) AttributLocations::MVP, MVP);
    _program->setUniform("light.direction", light.direction);
    _program->setUniform("light.color", light.color);

    _vao->bind();
    glDrawElements(GL_TRIANGLES, _ibo->size(), GL_UNSIGNED_INT, nullptr);
    _vao->unbind();
}

}  // namespace bgl
