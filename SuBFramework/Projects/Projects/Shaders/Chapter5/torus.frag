#version 430 core

in vec3 pos;
in vec2 textCoord;

out vec4 fragColor;

uniform sampler2D texture1;

void main() {

  //fragColor = texture(texture1, textCoord);
  fragColor = vec4(pos, 1.0f);

}
