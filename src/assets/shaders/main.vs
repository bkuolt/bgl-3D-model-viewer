#version 330 core

uniform mat4 MVP;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texcoords;

out vec3 pixelNormal;
out vec2 pixelTexCoord;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    pixelNormal = normalize(mat3(MVP) * normal);
    pixelTexCoord = texcoords;
}
