// Copyright 2020 Bastian Kuolt
#version 450 core

layout (location = 0) uniform mat4 MVP;
layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texcoords;
layout (location = 4) uniform sampler2D texture;  /* not used yet */

out flat vec3 L;
out vec3 N;

void main() {
    const vec4 light = vec4(-1.0f, -1.0f, -1.0f, 0.0f);

    L = vec3(light* MVP);
    N = normalize(mat3(MVP) * normal);
    gl_Position = MVP * vec4(position, 1.0);
}
