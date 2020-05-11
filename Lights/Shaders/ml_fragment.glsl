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

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 cameraPosition;


uniform Material material;


uniform DirectionalLight dirLight;
uniform PointLight pointLights[4];
uniform SpotLight spotlight;

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

vec3 calculatePointLights(vec3 diffuse, vec3 specular, vec3 n, vec3 e) {
    vec3 totalColor = vec3(0, 0, 0);
    for(int i = 0;i < 1; ++i) {
        vec3 l = pointLights[i].position - position;
        Light light = calculateLight(pointLights[i].light, diffuse, specular, n, e, l);

        float dist = length(l);
        float attenuation = 1.0f / (pointLights[i].constant + pointLights[i].linear * dist + pointLights[i].quadratic * dist * dist);

        totalColor += light.ambient + (light.diffuse + light.specular) * attenuation;
    }

    return totalColor;
}

vec3 calculateSpotlight(vec3 diffuse, vec3 specular, vec3 n, vec3 e) {
    vec3 l = normalize(spotlight.position - position);
    vec3 spotlightDirection = normalize(-spotlight.direction);

    float theta = dot(l, spotlightDirection);
    float eps = spotlight.innerAngle - spotlight.outerAngle;

    float k = smoothstep(0.0f, 1.0f, (theta - spotlight.outerAngle) / eps);

    float dist = length(l);
    float attenuation = 1.0f / (spotlight.constant + spotlight.linear * dist + spotlight.quadratic * dist * dist);
    Light light = calculateLight(spotlight.light, diffuse, specular, n, e, l);

    return light.ambient + (light.diffuse + light.specular) * k * attenuation;
}

void main() {

    vec3 n = normalize(normal);
    vec3 e = normalize(cameraPosition - position);
    vec3 diffuse = vec3(texture(material.diffuse, texPos));
    vec3 specular = vec3(texture(material.specular, texPos));

    outColor = vec4(calculateDirectionalLight(diffuse, specular, n, e) +
                    calculatePointLights(diffuse, specular, n, e) +
                    calculateSpotlight(diffuse, specular, n, e), 1.0f);
}
