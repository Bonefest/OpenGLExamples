#version 330 core

layout(location = 0) in vec3 vegPosition;
layout(location = 1) in vec2 vegTexPosition;

out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * model * vec4(vegPosition, 1.0f);
    texCoord = vegTexPosition;
}
