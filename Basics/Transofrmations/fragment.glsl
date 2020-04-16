#version 330 core

in vec2 texPos;
in vec3 color;

out vec4 outColor;

uniform sampler2D tex1;
uniform sampler2D tex2;

uniform float t;

void main() {
	outColor = vec4(mix(texture(tex1, texPos), texture(tex2, texPos), t));

}
