#version 330 core

layout (location = 0) in vec3 inVert;
layout (location = 1) in vec3 inClr;
layout (location = 2) in vec2 inTex;

out vec2 texPos;
out vec3 color;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(inVert, 1.0);
	texPos = inTex;
	color = inClr;
}
