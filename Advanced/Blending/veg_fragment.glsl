#version 330 core

out vec4 resultColor;

in vec2 texCoord;

uniform sampler2D texture1;

void main() {
    resultColor = texture(texture1, texCoord);
}
