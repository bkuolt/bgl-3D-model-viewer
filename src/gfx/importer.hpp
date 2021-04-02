#ifndef GFX_IMPORTER_HPP_
#define GFX_IMPORTER_HPP_

#include <memory>
#include <filesystem>


namespace bgl {

std::shared_ptr<Model> LoadModel(const std::filesystem::path &path);

}

#endif  // GFX_IMPORTER_HPP_
