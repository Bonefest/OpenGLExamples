#version 330 core

in vec2 texCrd;

out vec4 outColor;

uniform sampler2D texture1;

void main() {
    outColor = texture(texture1, texCrd);
}
