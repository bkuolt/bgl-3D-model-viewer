#version 330 core
// Copyright 2020 Bastian Kuolt

#extension GL_ARB_explicit_uniform_location : require

uniform mat4 MVP;
uniform vec3 color;

void main() {
    gl_FragColor = vec4(color, 1.0);
}
