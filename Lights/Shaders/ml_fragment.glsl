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
};


struct DirectionalLight {
    vec3 direction;

    Light light;
};

struct PointLight {
    vec3 position;

    Light light;

    float constant;
    float linear;
    float quadratic;
};

struct SpotLight {
    vec3 position;
    vec3 direction;

    float outerAngle;
    float innerAngle;

    Light light;
};

uniform vec3 cameraPosition;


uniform Material material;


uniform DirectionalLight dirLight;
uniform PointLight pointLights[4];
uniform SpotLight spotLight;

vec3 calculateMirrored(vec3 n, vec3 l) {
    return normalize(2 * n * dot(n, l) - l);
}

Light calculateLight(Light light, vec3 diffuse, vec3 specular, vec3 n, vec3 e, vec3 l) {
    // Diffuse
    float diffuseFactor = max(dot(l, n), 0);
    Light resultLight;
    resultLight.diffuse = diffuse * light.diffuse * diffuseFactor;

    // Specular
    vec3 d = calculateMirrored(n, l);
    float specularFactor = pow(max(dot(e, d), 0), material.shininess);

    resultLight.specular = specular * light.specular * specularFactor;

    // Ambient
    resultLight.ambient = diffuse * light.ambient;

    return resultLight;
}

vec3 calculateDirectionalLight(vec3 diffuse, vec3 specular, vec3 n, vec3 e) {
    vec3 l = normalize(-dirLight.direction);
    Light light = calculateLight(dirLight.light, diffuse, specular, n, e, l);
    return light.diffuse + light.specular + light.ambient;
}

vec3 calculatePointLight(vec3 diffuse, vec3 specular, vec3 n, vec3 e) {
    for(int i = 0;i < 4; ++i) {
        vec3 l = pointLights[i].position - position;


    }

    return vec3(0, 0, 0);
}

void main() {

    vec3 n = normalize(normal);
    vec3 e = normalize(cameraPosition - position);
    vec3 diffuse = vec3(texture(material.diffuse, texPos));
    vec3 specular = vec3(texture(material.specular, texPos));

    outColor = vec4(calculateDirectionalLight(diffuse, specular, n, e), 1.0f);
}
