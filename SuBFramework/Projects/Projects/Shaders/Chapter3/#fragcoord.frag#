#version 410 core

out vec4 outColor;

void main() {
  outColor = vec4(sin(gl_FragCoord.x * 0.25),
	  cos(gl_FragCoord.y * 0.25),
	  sin(gl_FragCoord.x * 0.25) * cos(gl_FragCoord.x),
	  1.0);
}
