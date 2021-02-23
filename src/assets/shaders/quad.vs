#version 450 core  // Copyright 2020 Bastian Kuolt

in vec2 vertex;

out gl_PerVertex { vec4 gl_Position; };
out vec2 texcoords;


void main() {
    gl_Position = vec4(vertex, 0, 1.0);
    texcoords = (vertex * 0.5) + vec2(0.5, 0.5);
}
