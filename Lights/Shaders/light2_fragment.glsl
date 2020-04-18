#version 330 core

in vec3 position;
in vec3 normal;

out vec4 outColor;

uniform float ambientFactor;

uniform vec3 objectColor;
uniform vec3 sourceColor;
uniform vec3 sourcePosition;

void main() {

    vec3 n = normalize(normal);
    vec3 l = normalize(sourcePosition - position);
    vec3 e = normalize(-position);
    vec3 d = normalize(2 * n * dot(n, l) - l);

    float dist = distance(sourcePosition, position);

    vec3 diffuse = objectColor * sourceColor * max(dot(n, l), 0.0f) * max(1.0f - min(dist/5.0f, 1.0f), 0.0f);
    vec3 specular = objectColor * sourceColor * pow(max(dot(d, e), 0.0f), 16.0f) * max(1.0f - min(dist/5.0f, 1.0f), 0.0f);
    vec3 ambient = objectColor * sourceColor * ambientFactor;
    outColor = vec4(ambient + diffuse + specular, 1.0f);
}
