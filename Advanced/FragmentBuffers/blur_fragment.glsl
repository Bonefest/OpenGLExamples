#version 330 core

in vec2 texPos;
out vec4 outColor;

uniform sampler2D texture;

void main() {
    float offset = 0.01;
    float factors[9] = float[](
        1, 2, 1,
        2, 8, 2,
        1, 2, 2
    );

    vec2 offsets[9] = vec2[](
        texPos - vec2(offset, offset),  texPos - vec2(0, offset), texPos + vec2(offset, -offset),
        texPos - vec2(offset, 0),       texPos,                   texPos + vec2(offset, 0),
        texPos + vec2(-offset, offset), texPos + vec2(0, offset), texPos + vec2(offset, offset)
    );

    float sum = 0.0f;
    vec3 resultColor = vec3(0);
    for(int i = 0;i < 9; ++i) {
        resultColor += vec3(texture2D(texture, offsets[i])) * factors[i];
        sum += factors[i];
    }

    resultColor /= sum;

    outColor = vec4(resultColor, 1.0);
}
