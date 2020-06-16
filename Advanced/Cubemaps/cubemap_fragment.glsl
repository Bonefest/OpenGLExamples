#version 330 core

in vec3 texPos;

out vec4 texColor;

uniform samplerCube texCube;

void main() {
    texColor = textureCube(texCube, texPos);
}
