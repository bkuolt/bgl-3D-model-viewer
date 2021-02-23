#version 330 core

#extension GL_ARB_explicit_uniform_location : require

layout (location = 0) uniform mat4 MVP;
layout (location = 1) uniform vec3 color;

void main() {
    gl_FragColor = vec4(color, 1.0);
}
