#version 430 core

in vec2 tc;
flat in int ai;

layout(binding = 0) uniform sampler2DArray tex;

out vec4 fragColor;

void main() {
  fragColor = texture(tex, vec3(tc, float(ai)));
  //fragColor = vec4(tc.xy, 0.5, 1.0);
}
