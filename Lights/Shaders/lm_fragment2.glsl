#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texPos;

out vec4 outColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;
};

uniform vec3 cameraPosition;

uniform Material material;
uniform Light light;

/*
 * Main differences between material and basic light chapters are splitting colors and lights intensity into three separate
 * variables - for ambient, diffuse and specular parts. In that way we can control viewing of objects much wider and create
 * better experience.
 */

void main() {
    vec3 n = normalize(normal);
    vec3 l = normalize(light.position - position);
    vec3 e = normalize(cameraPosition - position);
    vec3 d = normalize(2 * n * dot(n, l) - l);

    float dist = distance(light.position, position);

    vec3 fragmentColor = vec3(texture(material.diffuse, texPos));

    vec3 diffuse = fragmentColor * light.diffuse * max(dot(n, l), 0.0f) * max(1.0f - min(dist/5.0f, 1.0f), 0.0f);
    vec3 specular = (vec3(1, 1, 1) - vec3(texture(material.specular, texPos))) * light.specular * pow(max(dot(d, e), 0.0f), material.shininess) * max(1.0f - min(dist/5.0f, 1.0f), 0.0f);
    vec3 ambient = fragmentColor * light.ambient;
    outColor = vec4(ambient + diffuse + specular, 1.0f);
}
