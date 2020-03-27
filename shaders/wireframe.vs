// Copyright 2020 Bastian Kuolt
#version 450 core

layout (location = 0) uniform mat4 MVP;
layout (location = 1) uniform vec3 color;
layout (location = 2) in vec3 position;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
}
