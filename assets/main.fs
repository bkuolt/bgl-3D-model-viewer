#version 150 core

//layout (location = 4) uniform sampler2D texture;
in float intensity;

void main() {
    gl_FragColor = vec4(0.1, 0.1, 0.1, 1) + vec4(0.0, 1.0, 1.0, 1.0) * intensity;
}
