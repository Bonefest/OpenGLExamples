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
    vec3 direction;

    float innerAngle;
    float outerAngle;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 cameraPosition;

uniform Material material;
uniform Light light;

void main() {

    vec3 n = normalize(normal);

    vec3 l = normalize(light.position - position);
    vec3 ld = normalize(-light.direction);

    /*
     * I = (theta - gamma) / (fi - gamma) = (theta - gamma) / epsilon
     *
     * theta - angle between spotlight direction and light
     * gamma - outer spotlight angle
     * fi - inner spotlight angle
     * epsilon - distance from inner angle to outer
     *
     * Thanks to clamp the function becomes much clearer:
     *
     * If our angle between spotlight direction and light direction is out of outer angle then
     * theta - gamma be < 0 (cause it's moving from 1.0 to 0.0f due to cos and as result
     * will be lower than gamma) and as result it'll be clamped to 0.0
     *
     * If our angle will be between inner and outer cosines values then theta - gamma value
     * will be in the same range as fi - gamma and it's division will yield values in range
     * [0.0f, 1.0f]
     *
     * If our angle will be lower than inner cosines value then theta - gamma value will be
     * greater than fi - gamma and as result it will be greater than 1 and it will be clamped
     * to 1.0f!
     *
     * It's better to imagine like moving theta from 0.0f to gamma then to fi and then to 1.0 -
     * the value will be always growing and simultaneously clamping in range [0, 1]
     */

    float theta = dot(l, ld);
    float epsilon = light.innerAngle - light.outerAngle;
    float attenuation = smoothstep(0.0f, 1.0f, (theta - light.outerAngle) / epsilon);

    vec3 e = normalize(cameraPosition - position);
    vec3 d = normalize(2 * n * dot(n, l) - l);

    vec3 fragmentColor = vec3(texture(material.diffuse, texPos));

    vec3 diffuse = fragmentColor * light.diffuse * max(dot(n, l), 0.0f);
    vec3 specular = vec3(texture(material.specular, texPos)) * light.specular * pow(max(dot(d, e), 0.0f), material.shininess);
    vec3 ambient = fragmentColor * light.ambient;
    outColor = vec4( ambient + (diffuse + specular) * attenuation, 1.0f);
}
