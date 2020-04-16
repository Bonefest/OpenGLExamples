#version 330 core

in vec3 position;
in vec3 normal;

out vec4 outColor;

uniform vec3 objectColor;
uniform vec3 sourceColor;
uniform vec3 ambientColor;
uniform vec3 sourcePosition;

void main() {
    vec3 l = normalize(sourcePosition - position);
    vec3 diffuse = max(cos(dot(l, normal)), 0.0f) * objectColor * sourceColor;

    outColor = vec4(diffuse + ambientColor, 1.0f);
}
