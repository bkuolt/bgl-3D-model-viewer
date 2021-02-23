#version 450 core  // Copyright 2020 Bastian Kuolt

in vec2 vertex;
out gl_PerVertex { vec4 gl_Position; };

void main() {
    gl_Position = vec4(vertex, 0, 1.0);
}
