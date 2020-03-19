// Copyright 2020 Bastian Kuolt
#ifndef GFX_GFX_HPP_
#define GFX_GFX_HPP_

#include "gl.hpp"
#include "shader.hpp"
#include "buffer.hpp"
#include "mesh.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>  // SDL_GLContext, SDL_Window

#include <ostream>      // std::ostream
#include <memory>       // std::shared_ptr
#include <filesystem>   // std::filesystem::path


namespace bgl {

using SharedWindow = std::shared_ptr<SDL_Window>;
using SharedContext = std::shared_ptr<SDL_GLContext>;

SharedWindow createFullScreenWindow();
SharedContext createGLContext(const SharedWindow &window);

std::ostream& operator<<(std::ostream &os, const vec2 &vector);

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

class Camera {
 public:
    Camera();
    Camera(const vec3 &position, const vec3 &viewCenter);
    Camera(const Camera&) = default;
    Camera(Camera&&) = default;
    virtual ~Camera() noexcept = default;

    Camera& operator=(const Camera&) = default;
    Camera& operator=(Camera&&) = default;

    void setPosition(const vec3 &position) noexcept;
    void setZoom(double factor = 1.0) noexcept;
    void setViewCenter(const vec3 &center) noexcept;

    const vec3& getPosition() const noexcept;
    const vec3& getViewCenter() const noexcept;
    double getZoom() const noexcept;

    mat4 getMatrix() const noexcept;

    void rotate(const vec2 degrees) noexcept;

 private:
    vec3 _position;
    vec3 _center;
    double _zoom { 1.0 };
};

}  // namespace bgl

#endif  // GFX_GFX_HPP_
