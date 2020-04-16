#version 330 core

in vec2 pos;

out vec4 color;

uniform vec4 ucolor;

void main() {
    color = vec4(pos.y, pos.y, pos.y, 1.0f);
}
