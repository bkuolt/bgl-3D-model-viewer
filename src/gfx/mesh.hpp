// Copyright 2020 Bastian Kuolt
#ifndef GFX_MESH_HPP
#define GFX_MESH_HPP

#include "gl.hpp"

#include <QOpenGLTexture>  // TODO: move to cpp file
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <iostream>
#include <filesystem>
#include <memory>
#include <string>

#include "BoundingBox.hpp"

struct aiScene;
struct aiMesh;


namespace bgl {

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 texcoords;
};


class Mesh {
 public:
   Mesh() {}  // TODO
   
   explicit Mesh(const std::filesystem::path &path);
   virtual ~Mesh() = default;
   
   virtual void create();
   virtual void render(const mat4 &MVP);

   const BoundingBox& getBoundingBox() const;

 private:
   void createVBO(const aiScene *scene);
   void createIBO(const aiMesh *mesh);
   void createVAO();
   void createShaderProgram();

 protected:
   std::shared_ptr<VertexArrayObject> _vao;
   std::shared_ptr<QOpenGLBuffer> _vbo;
   std::shared_ptr<QOpenGLBuffer> _ibo;
   std::shared_ptr<QOpenGLTexture> _texture;
   std::shared_ptr<QOpenGLShaderProgram> _program;
   
 private:
   BoundingBox _bounding_box;
   const std::filesystem::path _path;
};

using SharedMesh = std::shared_ptr<Mesh>;

inline SharedMesh LoadMesh(const std::string &path) {
   std::cout << "LoadMesh()" << std::endl;
     auto r = std::shared_ptr<bgl::Mesh>(new bgl::Mesh(path));
   std::cout << "Done loading" << std::endl;
   r->create();
   return r;
}

}  // namespace bgl

#endif  // GFX_MESH_HPP
