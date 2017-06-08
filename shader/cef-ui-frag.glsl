#version 330

uniform sampler2D texture;
// uniform float opacity;
in vec2 texcoord;


void main() {
  gl_FragColor = texture2D(texture, texcoord);
}
