// Copyright 2020 Bastian Kuolt
#version 450 core

struct Light {
    vec3 direction;
    vec3 color;
};

layout (location = 0) uniform mat4 MVP;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texcoords;
layout (location = 4) uniform sampler2D texture;  /* not supported yet */
layout (location = 5) uniform Light light;

out vec3 N;
out vec2 T;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    N = normalize(mat3(MVP) * normal);
    T = texcoords;
}
