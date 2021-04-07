/**
 * @file importer.hpp
 * @brief All 3D model and image import features.
 * 
 */
#ifndef GFX_IMPORTER_HPP_
#define GFX_IMPORTER_HPP_

#include <memory>
#include <filesystem>


namespace bgl {

/**
 * @brief Loads and creates an OpenGL texture from an image file.
 */
std::shared_ptr<QOpenGLTexture> LoadTexture(const std::filesystem::path &path);

/**
 * @brief Loads a 3D model from a given path.
 */
std::shared_ptr<Model> LoadModel(const std::filesystem::path &path);

}

#endif  // GFX_IMPORTER_HPP_
