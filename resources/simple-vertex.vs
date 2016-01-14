#version 120

attribute vec4 coord;
uniform float aspect;
uniform float fov;
varying vec2 texcoord;

uniform mat4 view_frustum;

mat4 ViewFrustum(float angle_of_view,
                 float aspect_ratio,
                 float z_near,
                 float z_far) {
  return mat4(vec4(1.0 / tan(angle_of_view), 0.0, 0.0, 0.0),
              vec4(0.0, aspect_ratio / tan(angle_of_view), 0.0, 0.0),
              vec4(0.0, 0.0, (z_far + z_near) / (z_far - z_near), 1.0),
              vec4(0.0, 0.0, -2.0 * z_far * z_near / (z_far - z_near), 0.0));
}

mat4 Frustum(float l, float r, float b, float t, float n, float f) {
  return mat4(
      vec4(2 * n / (r - l), 0.0, 0.0, 0.0),
      vec4(0.0, 2 * n / (t - b), 0.0, 0.0),
      vec4((r + l) / (r - l), (t + b) / (t - b), -(f + n) / (f - n), -1),
      vec4(0.0, 0.0, -2 * f * n / (f - n), 0));
}

mat4 Ortho(float l, float r, float b, float t, float n, float f) {
  return mat4(vec4(2.0 / (r - l), 0.0, 0.0, (l + r) / (l - r)),
              vec4(0.0, 2.0 / (t - b), 0.0, (b + t) / (b - t)),
              vec4(0.0, 0.0, 2.0 / (n - f), (n + f) / (n - f)),
              vec4(0.0, 0.0, 0.0, 1.0));
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
  // gl_Position = ViewFrustum(radians(fov), aspect, 0.5, 50.0) *
  //               Translate(0.0, 0.0, 3.0) * coord;
  // gl_Position = transpose(Ortho(0.0, 1920.0, 1680.0, 0.0, -1.0, 1.0)) *
  // Scale(800.0, 800.0, 100.0) * coord;
  gl_Position = view_frustum * Scale(800.0, 800.0, 0.0) * coord;

  texcoord = coord.xy * vec2(0.5) + vec2(0.5);
}
