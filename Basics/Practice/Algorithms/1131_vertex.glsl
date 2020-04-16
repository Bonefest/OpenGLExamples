#version 330 core

layout(location = 0) in vec2 vPos;

void main() {
    gl_Position = vec4(0.5f * vPos.x, 0.5f * vPos.y * 4/3, 1.0f, 1.0f);
}
