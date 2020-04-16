#version 330 core

out vec4 outColor;

uniform vec3 sourceColor;

void main() {
    outColor = vec4(sourceColor, 1.0);
}
