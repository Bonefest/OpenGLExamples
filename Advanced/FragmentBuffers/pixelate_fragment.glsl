#version 330 core

in vec2 texPos;
out vec4 outColor;

uniform sampler2D texture;

void main() {
    float rate = 100.0f;
    vec2 pos = floor(texPos * rate + 0.5f) / rate;
    outColor = texture2D(texture, pos);
}
