#version 330 core

layout(location = 0) in vec3 inVrt;
layout(location = 1) in vec3 inClr;
layout(location = 2) in vec2 inTexPos;

out vec3 clr;
out vec2 texPos;

uniform mat4 transform;
uniform mat4 proj;

void main() {
    gl_Position = proj * transform * vec4(inVrt, 1.0f);
    clr = inClr;
    texPos = inTexPos;
}
