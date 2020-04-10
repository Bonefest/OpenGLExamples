#version 330 core

layout(location = 0) in vec2 vPos;

uniform float scale;
uniform float offset;

void main() {
    gl_Position = vec4(scale * vPos.x + offset, scale * vPos.y, 1.0, 1.0);
}
