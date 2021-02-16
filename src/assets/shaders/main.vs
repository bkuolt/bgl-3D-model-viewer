#version 450 core
// Copyright 2020 Bastian Kuolt

struct Light {
    bool used;
    vec3 direction;
    vec3 color;
};

layout (location = 0) uniform mat4 MVP;
layout (location = 1) uniform bool isTextured;
layout (location = 2) uniform sampler2D texture;
layout (location = 3) uniform Light lights[5];

layout (location = 8) in vec3 position;
layout (location = 9) in vec3 normal;
layout (location = 10) in vec2 texcoords;

out vec3 pixelNormal;
out vec2 pixelTexCoord;

out gl_PerVertex { vec4 gl_Position; };

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    pixelNormal = normalize(mat3(MVP) * normal);
    pixelTexCoord = texcoords;
}
