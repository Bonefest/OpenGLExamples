#version 330 core

layout(location = 0) in vec3 inVertPos;
layout(location = 1) in vec2 inTexPos;

out vec2 texPos;

uniform mat4 projection;
uniform mat4 transform;

void main() {
    gl_Position = projection * transform * vec4(inVertPos, 1.0);

    texPos = inTexPos;
}
