#version 330 core

out vec3 fragColor;

void main() {
  vec3 positions[] = vec3[3](vec3(-0.5, -0.5, 0.5),
	             vec3(-0.5,  0.5, 0.5),
	             vec3( 0.5, -0.5, 0.5));

  vec3 colors[] = vec3[3](vec3(1.0, 0.0, 0.0),
	          vec3(0.0, 1.0, 0.0),
	          vec3(0.0, 0.0, 1.0));

  gl_Position = vec4(positions[gl_VertexID], 0.46);
  fragColor = colors[gl_VertexID];
  
}
