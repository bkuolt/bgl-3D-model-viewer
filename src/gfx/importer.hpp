#pragma once

/**
 * @file importer.hpp
 * @brief Asset loader for 3D mesh files (via Assimp) and image textures.
 */

#include <filesystem>
#include <memory>

class QOpenGLTexture;

namespace bgl {

class Model;

/**
 * @brief Loads and constructs an OpenGL texture from an image file.
 * @param path File path to the texture image.
 * @return Shared pointer to the allocated QOpenGLTexture.
 */
std::shared_ptr<QOpenGLTexture> LoadTexture(const std::filesystem::path& path);

/**
 * @brief Loads a 3D model from a supported model file format (OBJ, etc.).
 * @param path File path to the 3D model asset.
 * @return Shared pointer to the constructed Model.
 */
std::shared_ptr<Model> LoadModel(const std::filesystem::path& path);

}  // namespace bgl
