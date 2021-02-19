// Copyright 2021 Bastian Kuolt
#ifndef GFX_MESH_HPP
#define GFX_MESH_HPP

#include "gl.hpp"

#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <iostream>
#include <filesystem>
#include <memory>

#include "BoundingBox.hpp"


namespace bgl {

struct Material {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
	float shininess;
	std::shared_ptr<QOpenGLTexture> texture;
	// TODO: add more properties;
};

class BasicMesh {
 public:
	BasicMesh();
	virtual ~BasicMesh() noexcept = default;
	virtual void render(const mat4 &MVP) = 0;

	void resize(const vec3 &dimensions);
	const BoundingBox& getBoundingBox() const;

 protected:
	std::shared_ptr<QOpenGLBuffer> _vbo;
	std::shared_ptr<QOpenGLBuffer> _ibo;
	std::shared_ptr<VertexArrayObject> _vao;
	std::shared_ptr<QOpenGLShaderProgram> _program;

	BoundingBox _boundingBox;
	Material _material;
};

class Mesh : public BasicMesh {
 public:
   Mesh() = default;
   explicit Mesh(const std::filesystem::path &path);
   virtual ~Mesh() = default;

   void render(const mat4 &MVP) override;
};

}  // namespace bgl

#endif  // GFX_MESH_HPP
