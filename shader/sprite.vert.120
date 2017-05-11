#version 120

attribute vec4 coords;
attribute vec2 texcoords;

uniform mat4 projection;
uniform mat4 view;

uniform float x;
uniform float y;
uniform float z;
uniform float rx;
uniform float ry;
uniform float rz;
uniform float width;
uniform float height;

varying vec2 texcoord;

mat4 Scale(float x, float y, float z) {
  return mat4(vec4(x, 0.0, 0.0, 0.0),
              vec4(0.0, y, 0.0, 0.0),
              vec4(0.0, 0.0, z, 0.0),
              vec4(0.0, 0.0, 0.0, 1.0));
}

mat4 Translate(float x, float y, float z) {
  return mat4(vec4(1.0, 0.0, 0.0, 0.0),
              vec4(0.0, 1.0, 0.0, 0.0),
              vec4(0.0, 0.0, 1.0, 0.0),
              vec4(x, y, z, 1.0));
}

mat4 RotateOnX(float theta) {
  return mat4(vec4(1.0, 0.0, 0.0, 0.0),
              vec4(0.0, cos(theta), sin(theta), 0.0),
              vec4(0.0, -sin(theta), cos(theta), 0.0),
              vec4(0.0, 0.0, 0.0, 1.0));
}

mat4 RotateOnZ(float theta) {
  return mat4(vec4(cos(theta), -sin(theta), 0.0, 0.0),
              vec4(sin(theta), cos(theta), 0.0, 0.0),
              vec4(0.0, 0.0, 1.0, 0.0),
              vec4(0.0, 0.0, 0.0, 1.0));
}

void main() {
  gl_Position = projection * view * Translate(x, y, z) *
                RotateOnX(radians(rx + 180)) * RotateOnZ(radians(rz)) *
                Scale(width, height, 0) * coords;

  texcoord = texcoords;
}
