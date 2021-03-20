// Copyright 2021 Bastian Kuolt
#ifndef GL_BOX_HPP_
#define GL_BOX_HPP_

#include "gl.hpp"
#include "model.hpp"

namespace bgl {

class Box final : public Model {
 public:
	Box();
	explicit Box(const BoundingBox &boundingBox);
	void render(const mat4 &VP) override;
};

}  // namespace bgl

#endif  // GL_BOX_HPP_
