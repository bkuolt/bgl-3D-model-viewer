// Copyright 2020 Bastian Kuolt
#version 150 core  // gl_FragColor

struct Pixel {
    vec3 normal;
};

// layout (location = 4) uniform sampler2D texture;
in Pixel pixel;  /* not used yet */

float calculateLightIntensity() {
    const vec3 L = vec3(-1.0f, -1.0f, -1.0f);
    return max(dot(L, normalize(pixel.normal)), 0.0);
}

void main() {
    const vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f);
    const vec4 lightColor = vec4(1.0, 1.0, 0.0, 1.0);
    gl_FragColor = ambient + (lightColor * calculateLightIntensity());
}
