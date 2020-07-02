#version 330 core

layout(location = 0) in vec3 offset;
layout(location = 1) in vec3 color;

out vec3 fs_color;

void main() {

    fs_color = color;

    vec3 positions[] = vec3[3](vec3(-0.5, -0.5, -0.5),
                             vec3( 0.0,  0.5, -0.5),
                             vec3( 0.5, -0.5, -0.5));

    gl_Position = vec4(positions[gl_VertexID], 1.0);

}
