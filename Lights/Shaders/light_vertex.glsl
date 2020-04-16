#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

out vec3 normal;
out vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    gl_Position = proj * view * model * vec4(vPos, 1.0);
    normal = vec3(model * vec4(vNormal, 1.0));
    position = vec3(model * vec4(vPos, 1.0));
}
