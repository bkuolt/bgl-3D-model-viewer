#version 450 core
// Copyright 2020 Bastian Kuolt

layout (location = 0) uniform mat4 MVP;
layout (location = 1) uniform vec3 color;
layout (location = 2) in vec3 position;

out gl_PerVertex { vec4 gl_Position; };

void main() {
    gl_Position = MVP * vec4(position, 1.0);
}
