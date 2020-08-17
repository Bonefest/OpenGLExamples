#version 330 core

out vec4 outColor;

uniform sampler2D texture;

void main() {

  outColor = texelFetch(texture, ivec2(gl_FragCoord.xy), 0);

}
