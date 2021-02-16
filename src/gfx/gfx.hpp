// Copyright 2020 Bastian Kuolt
#ifndef GFX_GFX_HPP_
#define GFX_GFX_HPP_

#include "gl.hpp"
#include "mesh.hpp"

#include "gui/window.hpp"

#include <SDL2/SDL.h>   // SDL basic functionality

#include <filesystem>   // std::filesystem::path
#include <memory>       // std::shared_ptr
#include <ostream>      // std::ostream


namespace bgl {

std::shared_ptr<QOpenGLShaderProgram> LoadProgram(const std::filesystem::path &vs, const std::filesystem::path &fs);

#ifdef __linux
namespace console_color {
    constexpr char blue[] = "\x1B[34m";
    constexpr char red[] = "\x1B[31m";
    constexpr char white[] = "\x1B[37m";
    constexpr char magenta[] = "\x1B[35m";
    constexpr char yellow[] = "\x1B[33m";
    constexpr char green[] =  "\x1B[32m";
}  // namespace console_color

#endif  // __linux



// TODO(bkuolt): move to separate header
class grid final {
 public:
    explicit grid(GLfloat size, std::size_t num_cells);
    void render(const mat4 &MVP);

    void translate(const vec3 &v) {
        _translation += v;
    }

 private:
    const GLfloat _cell_size;
    const std::size_t _num_cells;

    vec3 _translation;

    void create_vbo();
    void create_ibo();
    void create_vao();

   std::shared_ptr<VertexArrayObject> _vao;
   std::shared_ptr<QOpenGLBuffer> _vbo;
   std::shared_ptr<QOpenGLBuffer> _ibo;
    std::shared_ptr<QOpenGLShaderProgram> _program;
};

using SharedGrid = std::shared_ptr<grid>;

inline SharedGrid CreateGrid(GLfloat size, size_t num_cells) {
    return std::make_shared<grid>(size, num_cells);
}

}  // namespace bgl

#endif  // GFX_GFX_HPP_
