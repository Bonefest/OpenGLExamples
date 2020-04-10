#version 330 core

layout(location = 0) in vec3 inVertCrd;
layout(location = 1) in vec2 inTexCrd;

out vec2 texCrd;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * model * vec4(inVertCrd, 1.0f);
    texCrd = inTexCrd;
}
