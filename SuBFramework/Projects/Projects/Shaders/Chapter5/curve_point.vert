#version 330 core

uniform vec3 vertices[5];

void main() {

  gl_Position = vec4(vertices[gl_VertexID], 1.0);

}
