#version 330 core

in vec3 clr;
in vec2 texPos;

out vec4 oClr;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float t;

void main() {
    oClr = mix(texture(tex1, texPos), texture(tex2, texPos), 1.0f);
}
