#version 330 core

layout(location = 0) in vec2 vPos;

out vec2 pos;

void main() {
    float radius = 0.5f;
    gl_Position = vec4(vPos, 1.0f, 1.0f);
    pos = vPos;
}
