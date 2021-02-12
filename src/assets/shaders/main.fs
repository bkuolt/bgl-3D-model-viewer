// Copyright 2020 Bastian Kuolt
#version 330 core  // gl_FragColor
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_separate_shader_objects : require

#define MAX_LIGHTS 5

struct Light {
    bool used;
    vec3 direction;
    vec3 color;
};

layout (location = 0) uniform mat4 MVP;
layout (location = 1) uniform bool isTextured;
layout (location = 2) uniform sampler2D texture;
layout (location = 3) uniform Light lights[5];

in vec3 pixelNormal;
in vec2 pixelTexCoord;

float calculateLightIntensity(in Light light) {
    return max(dot(light.direction, normalize(pixelNormal)), 0.0);
}

vec4 getLightColor() {
    const vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 0.0);

    vec4 color = ambient;
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        if (lights[i].used) {
            color += vec4(lights[i].color, 0.0) * calculateLightIntensity(lights[i]) * 0.8;
        }
    }
    return color;
}

void main() {
    gl_FragColor = getLightColor() *
        ((isTextured) ? texture2D(texture, pixelTexCoord) : vec4(1.0, 1.0, 1.0, 1.0));
}
