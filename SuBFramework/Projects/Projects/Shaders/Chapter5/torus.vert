#version 430 core

layout(location = 0) in vec4 inVert;
layout(location = 1) in vec3 normal;
//layout(location = 4) in vec2 inTextCoord;

uniform mat4 proj;
uniform mat4 view;
uniform mat4 model

out vec2 textCoord;

void main() {

  gl_Position = proj * view * model * vec4(inVert.xyz, 1.0f);

  textCoord = inTextCoord;

}