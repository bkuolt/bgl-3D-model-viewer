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

}  // namespace bgl

#endif  // GFX_GFX_HPP_
