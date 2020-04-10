#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vClr;

out vec3 clr;
uniform vec3 offset;

void main() {
	gl_Position = vec4(vPos + offset, 1.0);
	clr = gl_Position.xyz;
}
