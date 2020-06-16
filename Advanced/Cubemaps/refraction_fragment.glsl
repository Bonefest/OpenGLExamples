#version 330 core

in vec2 texposition;
in vec3 position;
in vec3 normal;

out vec4 color;

uniform vec3 cameraPosition;
uniform samplerCube texcube;
uniform sampler2D spectex;

void main() {
    vec3 eye = normalize(position - cameraPosition);
    color = textureCube(texcube, refract(eye, normalize(normal), 0.413));


}
