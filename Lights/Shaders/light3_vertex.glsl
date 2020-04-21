#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;

out vec4 vertColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

uniform float ambientFactor;

uniform vec3 objectColor;
uniform vec3 sourceColor;
uniform vec3 sourcePosition;
uniform vec3 cameraPosition;

void main() {
    vec3 normal = vec3(model * vec4(vNormal, 0.0));
    vec3 position = vec3(model * vec4(vPos, 1.0));

    vec3 n = normalize(normal);
    vec3 l = normalize(sourcePosition - position);
    vec3 e = normalize(cameraPosition - position);
    vec3 d = normalize(2 * n * dot(n, l) - l);



    float dist = distance(sourcePosition, position);

    vec3 diffuse = objectColor * sourceColor * max(dot(n, l), 0.0f);
    vec3 specular = objectColor * sourceColor * pow(max(dot(d, e), 0.0f), 16.0f);
    vec3 ambient = objectColor * sourceColor * ambientFactor;
    vertColor = vec4(ambient + diffuse + specular, 1.0f);


    gl_Position = proj * view * model * vec4(vPos, 1.0);
}
