// Copyright 2020 Bastian Kuolt
#version 330 core  // gl_FragColor
#extension GL_ARB_explicit_uniform_location : require

struct Light {
    vec3 direction;
    vec3 color;
};

layout (location = 4) uniform sampler2D texture;  /* not supported yet */
layout (location = 5) uniform Light light;

in vec3 N;

float calculateLightIntensity() {
    // TODO(bkuolt): use data from light struct
    const vec3 L = vec3(-1.0f, -1.0f, -1.0f);
    return max(dot(L, normalize(N)), 0.0);
}

void main() {
    const vec4 ambient = vec4(0.3f, 0.3f, 0.3f, 1.0f);
    const vec4 lightColor = vec4(0.5, 0.5, 0.0, 1.0);
    // TODO(bkuolt): use data from light struct
    gl_FragColor = ambient + (lightColor * calculateLightIntensity());
}
