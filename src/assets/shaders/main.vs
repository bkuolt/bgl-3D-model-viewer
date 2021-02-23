#version 450 core

uniform mat4 MVP;

uniform struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;

in vec3 position;
in vec3 normal;
in vec2 texcoords;

out vec3 pixelNormal;
out vec2 pixelTexCoord;
out gl_PerVertex { vec4 gl_Position; };


void main() {
    gl_Position = MVP * vec4(position, 1.0);
    pixelNormal = normalize(mat3(MVP) * normal);
    pixelTexCoord = texcoords;
}
