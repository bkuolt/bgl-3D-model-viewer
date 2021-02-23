#version 330 core
// Copyright 2020 Bastian Kuolt
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_separate_shader_objects : require

uniform mat4 MVP;

uniform struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
} light;

uniform struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    bool isTextured;
    sampler2D texture;
} material;

in vec3 pixelNormal;
in vec2 pixelTexCoord;


float calculateLightIntensity() {
    return max(dot(light.direction, normalize(pixelNormal)), 0.0);
}

vec4 getLightColor() {
    vec3 color = light.ambient;
    color += light.diffuse * calculateLightIntensity() * 0.8;
    return vec4(color, 0.0);
}

void main() {
    gl_FragColor = getLightColor() *
        ((material.isTextured) ? texture2D(material.texture, pixelTexCoord) : vec4(1.0, 1.0, 1.0, 1.0));
}
