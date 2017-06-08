#version 330

in vec4 coords;
in vec2 texcoords;

uniform mat4 projection;
uniform mat4 view;

out vec2 texcoord;

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
  gl_Position = projection * view * coords;
  texcoord = texcoords;
}
