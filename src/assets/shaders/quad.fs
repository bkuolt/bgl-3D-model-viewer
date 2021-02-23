#version 330 core  // Copyright 2020 Bastian Kuolt

uniform vec3 color;

void main() {
    gl_FragColor = vec4(color, 1.0);
}
