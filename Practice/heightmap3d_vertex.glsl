#version 330 core

layout(location = 0) in vec3 vPos;

uniform mat4 projection;
uniform mat4 transform;

out vec3 pos;

void main() {
    gl_Position = projection * transform * vec4(vPos, 1.0);
    pos = vPos;
}
