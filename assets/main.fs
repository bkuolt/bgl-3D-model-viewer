// Copyright 2020 Bastian Kuolt
#version 330 core  // gl_FragColor
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_separate_shader_objects : require

struct Light {
    vec3 direction;
    vec3 color;
};

layout (location = 4) uniform sampler2D texture;
layout (location = 5) uniform Light light;

in vec3 N;
in vec2 T;

float calculateLightIntensity() {
    return max(dot(light.direction, normalize(N)), 0.0);
}

void main() {
    const vec4 ambient = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    gl_FragColor = ambient + (vec4(light.color, 1.0) * calculateLightIntensity());  // texture2D(texture, T);
}
