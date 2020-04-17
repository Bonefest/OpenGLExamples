#version 330 core

in vec3 position;
in vec3 normal;

out vec4 outColor;

uniform vec3 objectColor;
uniform vec3 sourceColor;
uniform vec3 ambientColor;
uniform vec3 sourcePosition;

void main() {
    vec3 n = normalize(normal);
    vec3 l = normalize(sourcePosition - position);
    float dist = distance(sourcePosition, position);

    vec3 diffuse = objectColor * sourceColor * max(dot(n, l), 0.0f) * max(1.0f - min(dist/5.0f, 1.0f), 0.0f);

    outColor = vec4(ambientColor + diffuse, 1.0f);
}
