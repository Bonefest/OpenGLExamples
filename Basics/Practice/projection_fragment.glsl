#version 330 core

in vec2 texPos;

out vec4 outColor;

uniform sampler2D texture1;

void main() {
    outColor = texture(texture1, texPos);
}

