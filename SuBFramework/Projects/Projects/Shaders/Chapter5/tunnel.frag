#version 330 core

in vec2 texPos;

out vec4 fragColor;

uniform sampler2D text;

void main() {
  fragColor = texture(text, texPos);
}
