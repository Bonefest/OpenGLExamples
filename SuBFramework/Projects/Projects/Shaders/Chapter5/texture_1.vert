#version 330 core

layout(location = 0) in vec3 inVertPos;

void main() {
  gl_Position = vec4(inVertPos, 1.0);
}
