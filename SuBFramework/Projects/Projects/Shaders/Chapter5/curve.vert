#version 330 core

uniform vec3 vertices[5];
uniform int vertices_number;

vec3 quadraticBezier(vec3 pointA, vec3 pointB, vec3 pointC, float t) {

  vec3 pointE = mix(pointA, pointB, t);
  vec3 pointD = mix(pointB, pointC, t);
  vec3 pointF = mix(pointE, pointD, t);

  return pointF;
}

vec3 cubicBezier(vec3 pointA, vec3 pointB, vec3 pointC, vec3 pointD, float t) {

  vec3 pointE = mix(pointA, pointB, t);
  vec3 pointF = mix(pointB, pointC, t);
  vec3 pointG = mix(pointC, pointD, t);

  return quadraticBezier(pointE, pointF, pointG, t);

}

void main() {

  float tVal = 0.0f;
  if(vertices_number > 1) {
    tVal = float(gl_VertexID) / float(vertices_number - 1);
  }

  // gl_Position = vec4(quadraticBezier(vertices[0],
  //   vertices[1],
  //   vertices[2],
  //   tVal), 1.0);

  gl_Position = vec4(cubicBezier(
                                 vertices[0],
                                 vertices[1],
                                 vertices[2],
                                 vertices[3],
                                 tVal
                                 ), 1.0);
}
