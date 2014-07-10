#version 110

attribute vec4 position;
uniform float aspect;
uniform float fov;
varying vec2 texcoord;

mat4 ViewFrustum(float angle_of_view,
                 float aspect_ratio,
                 float z_near,
                 float z_far) {
  return mat4(vec4(1.0 / tan(angle_of_view), 0.0, 0.0, 0.0),
              vec4(0.0, aspect_ratio / tan(angle_of_view), 0.0, 0.0),
              vec4(0.0, 0.0, (z_far + z_near) / (z_far - z_near), 1.0),
              vec4(0.0, 0.0, -2.0 * z_far * z_near / (z_far - z_near), 0.0));
}

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
  gl_Position = ViewFrustum(radians(fov), aspect, 0.5, 5.0) *
                Translate(0.0, 0.0, 3.0) * position;
  texcoord = position.xy * vec2(0.5) + vec2(0.5);
}
