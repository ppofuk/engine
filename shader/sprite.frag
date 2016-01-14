#version 120

uniform sampler2D texture;
uniform float opacity;
varying vec2 texcoord;

void main() {
  gl_FragColor = texture2D(texture, texcoord) * vec4(1.0f, 1.0f, 1.0f, opacity);
}
