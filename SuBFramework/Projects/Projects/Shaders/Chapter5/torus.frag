#version 430 core

in vec2 textCoord;

out vec4 fragColor;

uniform sampler2D texture1;

void main() {

  fragColor = texture(texture1, textCoord * 2.0);

}
