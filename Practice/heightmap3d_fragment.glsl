#version 330 core

in vec3 pos;

out vec4 color;

uniform vec4 ucolor;

void main() {
    color = vec4(0.5f, pos.y, 0.5f, 1.0f) + ucolor;
}
