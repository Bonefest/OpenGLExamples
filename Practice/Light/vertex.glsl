#version 330 core

layout(location = 0) in vec3 inVrt;
layout(location = 1) in vec2 inTexPos;
layout(location = 2) in vec3 inNormal;

out vec3 clr;
out vec2 texPos;

out vec4 vspec;
out vec4 vdiff;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 cameraPosition;

void main() {
    vec4 transformedNormal = normalize(model * vec4(inNormal, 1.0f));

    float glfact = 20.0f;
    vec4 sourceLight = vec4(2.0f, 1.0f, 1.0f, 1.0f);
    vec4 sourcePosition = vec4(-2.0f, 5.0f, 0.0f, 1.0f);
    vec4 v = normalize(vec4(cameraPosition, 1.0) - model * vec4(inVrt, 1.0));
    vec4 l = normalize(sourcePosition - model * vec4(inVrt, 1.0));
    vec4 r = normalize(2*dot(l, transformedNormal)*transformedNormal - l);

    vspec = sourceLight * pow(max(dot(r, v), 0.0f), glfact);
    vdiff = sourceLight * max(dot(l, transformedNormal), 0.0f) + vec4(0.2f, 0.2f, 0.2f, 1.0f);


    gl_Position = projection * view * model * vec4(inVrt, 1.0f);
    texPos = inTexPos;
}
