#version 330 core

const float n = 0.1f;
const float f = 100.0f;

void main() {
    float z = gl_FragCoord.z * 2.0f - 1.0f;
    float zz = (-2*f*n / (f - n)) / ((-f - n) / (f - n) + z);
    zz = zz  / (f - n);
    gl_FragColor = vec4(vec3(zz), 1.0f);
}
