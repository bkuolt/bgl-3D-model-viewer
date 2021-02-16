// Copyright 2021 Bastian Kuolt
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

	void resize(const vec3 &dimensions);   // move to mesh

 private:
	 BoundingBox _boundingBox;
};

}  // namespace bgl

#endif  // GL_BOX_HPP_
