#version 430 core

layout(location = 0) in vec3 inPos;

layout(std140) uniform Matrices {

  mat4 projection;
  mat4 view;

};

layout(location = 1) uniform mat4 model;

void main() {

  gl_Position = projection * view * model * vec4(inPos, 1.0);

}
