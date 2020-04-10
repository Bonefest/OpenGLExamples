#version 330 core

layout(location = 0) in vec3 inVert;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexPos;

out vec2 texPos;
out vec3 color;
uniform vec2 offset;

void main() {
	gl_Position = vec4(inVert.xy + offset, inVert.z, 1.0);
	color = inColor;
	texPos = inTexPos;
}