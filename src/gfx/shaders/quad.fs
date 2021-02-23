#version 330 core  // Copyright 2020 Bastian Kuolt

uniform sampler2D texture;
in vec2 texcoords;


void main() {
    gl_FragColor = texture2D(texture, texcoords);  // just as "red filter test": + vec4(0.5, 0, 0, 0);
}
