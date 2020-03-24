// Copyright 2020 Bastian Kuolt
#version 330 core  // gl_FragColor
#extension GL_ARB_explicit_uniform_location : require
#extension GL_ARB_separate_shader_objects : require

#define MAX_LIGHT_COUNT 5

struct Light {
    bool used;
    vec3 direction;
    vec3 color;
};

uniform bool isTextured;
layout (location = 4) uniform sampler2D texture;  /* not supported yet */
layout (location = 5) uniform Light lights[MAX_LIGHT_COUNT];

in vec3 N;
in vec2 T;

float calculateLightIntensity(in Light light) {
    return max(dot(light.direction, normalize(N)), 0.0);
}

vec3 getLightColor(in Light light) {
    if (!light.used) {
        return vec3(0.0f, 0.0f, 0.0f);
    }
    return light.color * calculateLightIntensity(light);
}        

void main() {
    const vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 0.0);

    vec4 color = ambient;
    for (int i = 0; i < MAX_LIGHT_COUNT; ++i) {
        color += vec4(getLightColor(lights[i]), 0.0) * 0.3;
    }

    gl_FragColor = color + ((isTextured) ? texture2D(texture, T) : vec4(0));
}
