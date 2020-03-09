#version 450 core
layout (location = 0) uniform mat4 MVP;

layout (location = 1) in vec3 position;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 texcoords;
layout (location = 4) uniform sampler2D texture;

out float intensity;

void main() {
    gl_Position = MVP * vec4(position, 1.0);
    const vec3 lightDirection = vec3(-1,-1,-1);
    intensity = dot(lightDirection,  normal);
}
