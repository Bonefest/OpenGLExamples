#version 430 core

layout(location = 0) in vec3 inVertPos;
layout(location = 1) in vec2 inTexPos;

out vec2 texPos;

layout(location = 0) uniform mat4 proj;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 model;

void main() {
  gl_Position = proj * view * model * vec4(inVertPos, 1.0f);
  texPos = inTexPos;
}
