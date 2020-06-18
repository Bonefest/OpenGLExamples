#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texPos;

out vec4 outColor;

uniform float screenWidth;


void main() {

    if(gl_FragCoord.x <= screenWidth * 0.5)
        discard;

    outColor = vec4(1.0, 1.0, 1.0, 1.0);
}
