// Copyright 2020 Bastian Kuolt
#version 150 core  // gl_FragColor
in vec3 N;
in vec3 L;


const float PI_4 = 0.785398163397448309616;

float calculateLightIntensity(in vec3 L, in vec3 N) {
    N = normalize(N);
    L = -normalize(L);
    return max(dot(N, L), 0) / PI_4;
}

void main() {
    const vec4 lightColor = vec4(0.5, 0.5, 0.0, 1.0);
    const float ambient = 0.1;

    float intensity = calculateLightIntensity(N, L);
    gl_FragColor = vec4(vec3(ambient), 0) + lightColor * intensity;
}
