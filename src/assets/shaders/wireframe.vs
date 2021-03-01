#version 450 core
// Copyright 2020 Bastian Kuolt

uniform mat4 MVP;
uniform vec3 color;
in vec3 position;

out gl_PerVertex { vec4 gl_Position; };

void main() {
    gl_Position = MVP * vec4(position, 1.0);
}
