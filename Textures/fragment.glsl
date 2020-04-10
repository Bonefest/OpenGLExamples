#version 330 core

in vec2 texPos;
in vec3 color;
out vec4 outColor;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float t;

void main() {
	outColor = mix(texture(tex2, vec2(1.0 - texPos.x, texPos.y)), texture(tex1, texPos), t) * vec4(color, 1.0f);
}