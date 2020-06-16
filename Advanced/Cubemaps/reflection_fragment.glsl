#version 330 core

in vec2 texposition;
in vec3 position;
in vec3 normal;

out vec4 color;

uniform vec3 cameraPosition;
uniform samplerCube texcube;
uniform sampler2D spectex;

void main() {
    vec3 eye = normalize(position - cameraPosition);
    vec4 speccolor = texture2D(spectex, texposition);
    color = vec4(1.0, 0.0, 0.0, 1.0);
    if(speccolor.r != 0.0)
        color = textureCube(texcube, reflect(eye, normalize(normal)));
    else
        discard;


}
