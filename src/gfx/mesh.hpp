// Copyright 2021 Bastian Kuolt
#ifndef GFX_MESH_HPP_
#define GFX_MESH_HPP_

#include <optional>

#include "gl.hpp"

#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>


namespace bgl {

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoords;
};


class Mesh {
 public:
	Mesh();
	virtual ~Mesh() noexcept = default;

	void bind();
	void release();

	void render(GLenum mode, GLuint count);
	void render(GLenum mode);

	QOpenGLBuffer _vbo;
	QOpenGLBuffer _ibo;
	QOpenGLVertexArrayObject _vao;
	std::optional<unsigned int> _materialIndex;
};

}  // namespace bgl

#endif  // GFX_MESH_HPP_
