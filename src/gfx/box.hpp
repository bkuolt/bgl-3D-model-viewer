#ifndef GFX_BOX_HPP_
#define GFX_BOX_HPP_

#include "gl.hpp"
#include "model.hpp"


namespace bgl {

/**
 * @brief OpenGL renderable 3D box.
 */
class Box final : public Model {
 public:
	Box();
	explicit Box(const BoundingBox &boundingBox);
	// TODO

	void render(const mat4 &VP) override;
};

}  // namespace bgl

#endif  // GFX_BOX_HPP_
