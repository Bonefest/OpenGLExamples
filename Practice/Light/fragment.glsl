#version 330 core

out vec4 color;

in vec2 texPos;
in vec4 vspec;
in vec4 vdiff;

uniform sampler2D tex1;
uniform sampler2D tex2;

void main() {
    color = mix(texture(tex1, texPos), texture(tex2, texPos), 1.0f) * vdiff + vspec * vec4(1.0f, 1.0f, 1.0f, 1.0f);
}
