#version 330

// uniform sampler2D texture;
// uniform float opacity;
// in vec2 texcoord;
in vec2 texture_position_sampler; 
in vec3 normal_sampler; 
uniform sampler2D texture; 

void main() {
  gl_FragColor = texture2D(texture, vec2(texture_position_sampler.x, 1 - texture_position_sampler.y));
}
