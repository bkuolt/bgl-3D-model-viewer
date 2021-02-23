#ifndef GL_BOX_HPP_
#define GL_BOX_HPP_

#include "gl.hpp"
#include "mesh.hpp"

namespace bgl {

class Box final : public Mesh {
 public:
	Box();
	explicit Box(const BoundingBox &boundingBox);
	void render(const mat4 &VP) override;
};

}  // namespace bgl

#endif  // GL_BOX_HPP_
