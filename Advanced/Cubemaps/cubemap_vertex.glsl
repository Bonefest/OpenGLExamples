#version 330 core

layout(location = 0) in vec3 vertPos;

out vec3 texPos;

uniform mat4 proj;
uniform mat4 view;

void main() {
    gl_Position = proj * mat4(mat3(view)) * vec4(vertPos, 1.0);
    texPos = vertPos;
}
