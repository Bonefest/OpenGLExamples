#version 330 core

layout(location = 0) in vec3 inVertPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 fragColor;

void main() {
  gl_Position = projection * view * model * vec4(inVertPos, 1.0);

  fragColor = vec4((inVertPos + vec3(0.5f, 0.5f, 0.5f)) * 0.5 + vec3(0.1, 0.1, 0.1), 1.0);
}
