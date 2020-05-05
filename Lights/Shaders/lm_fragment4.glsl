#version 330 core

in vec3 position;
in vec3 normal;
in vec2 texPos;

out vec4 outColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
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

uniform float time;

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
    vec3 specularColor = vec3(texture(material.specular, texPos));
    vec3 emissionColor = vec3(0, 0, 0);
    if(specularColor == vec3(0, 0, 0)) {
        emissionColor = vec3(texture(material.emission, texPos + vec2(sin(time), 0))).ggg * (sin(4 * time) + 1.5) * 2;  //Thanks to .brg we move green to blue and as result change our emission color blue
    }

    vec3 light_diffuse = (light.diffuse + emissionColor);
    vec3 light_ambient = (light.ambient + emissionColor);

    vec3 diffuse = fragmentColor * light_diffuse * max(dot(n, l), 0.0f) * max(1.0f - min(dist/5.0f, 1.0f), 0.0f);
    vec3 specular = vec3(texture(material.specular, texPos)) * light.specular * pow(max(dot(d, e), 0.0f), material.shininess) * max(1.0f - min(dist/5.0f, 1.0f), 0.0f);
    vec3 ambient = fragmentColor * light_ambient;
    outColor = vec4(ambient + diffuse + specular, 1.0f);
}
