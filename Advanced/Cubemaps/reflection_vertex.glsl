#version 330 core

layout(location = 0) in vec3 vertPos;
layout(location = 1) in vec2 vertTexpos;
layout(location = 2) in vec3 vertNormal;

out vec2 texposition;
out vec3 position;
out vec3 normal;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;

void main() {
    gl_Position = proj * view * model * vec4(vertPos, 1.0);
    position = vec3(model * vec4(vertPos, 1.0));
    normal = vec3(model * vec4(vertNormal, 0.0));
    texposition = vertTexpos;
}
