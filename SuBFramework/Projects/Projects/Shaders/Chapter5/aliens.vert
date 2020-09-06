#version 430 core

layout(location = 0) in int alienIndex;

out vec2 tc;
flat out int ai;

struct Alien {
  float x;
  float y;
  float orientation;
  float pass;
};

layout(std140) uniform Aliens {
  Alien aliens[256];
};

void main() {

  vec2[4] positions = vec2[4](vec2(-0.5, -0.5),
                              vec2(-0.5,  0.5),
                              vec2( 0.5, -0.5),
                              vec2( 0.5,  0.5));

  tc = positions[gl_VertexID] + vec2(0.5, 0.5);
  ai = alienIndex % 64;

  float c = cos(aliens[alienIndex].orientation);
  float s = sin(aliens[alienIndex].orientation);

  mat2 rot = mat2(vec2(c, s), vec2(-s, c));

  vec2 alienCenter = vec2(aliens[alienIndex].x, aliens[alienIndex].y);

  gl_Position = vec4(rot * positions[gl_VertexID] * 0.25 + alienCenter, 0.0, 1.0);

}
