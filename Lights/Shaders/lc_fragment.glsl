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

    vec4 vector;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 cameraPosition;

uniform Material material;
uniform Light light;

void main() {

    vec3 n = normalize(normal);

    vec3 l;
    float attenuation = 1.0f;

    //If we are dealing with directional light
    if(light.vector.w < 0.01f) {
        //vec3 l = normalize(light.position - position); - inside direction light l is constant and passes through uniform!
        l = normalize(vec3(-light.vector));
    } else {
        float dist = distance(position, vec3(light.vector));
        l = normalize(vec3(light.vector) - position);
        attenuation = 1.0f / (light.constant + light.linear * dist + light.quadratic * dist * dist);
    }
    vec3 e = normalize(cameraPosition - position);
    vec3 d = normalize(2 * n * dot(n, l) - l);

    vec3 fragmentColor = vec3(texture(material.diffuse, texPos));

    vec3 diffuse = fragmentColor * light.diffuse * max(dot(n, l), 0.0f);
    vec3 specular = vec3(texture(material.specular, texPos)) * light.specular * pow(max(dot(d, e), 0.0f), material.shininess);
    vec3 ambient = fragmentColor * light.ambient;
    outColor = vec4( (ambient + diffuse + specular) * attenuation, 1.0f);
}
