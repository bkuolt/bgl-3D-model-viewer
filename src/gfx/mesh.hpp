#ifndef GFX_MESH_HPP_
#define GFX_MESH_HPP_

#include <optional>

#include "gl.hpp"

#include <QOpenGLBuffer>             // NOLINT
#include <QOpenGLVertexArrayObject>  // NOLINT


namespace bgl {

/**
 * @brief OpenGL VBO vertex format.
 */
struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoords;
};

/**
 * @brief Contains and manages all OpenGL resources (VBOs, IBOs, VAOs,
 *        shaders and textures) for a mesh.
 */
struct Mesh {
	Mesh();
	virtual ~Mesh() noexcept = default;

	void bind();
	void release();

	void render(GLenum mode, GLuint count);
	void render(GLenum mode);

	QOpenGLBuffer _vbo;
	QOpenGLBuffer _ibo;
	QOpenGLVertexArrayObject _vao;
	std::optional<unsigned int> _materialIndex;  // index to an Assimp material
};

}  // namespace bgl

#endif  // GFX_MESH_HPP_
