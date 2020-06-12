#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexPos;

out vec2 texPos;

void main() {
    gl_Position = vec4(inPos, 0.0f, 1.0f);
    texPos = inTexPos;
}
