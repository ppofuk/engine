#version 330

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texture_position;
layout(location = 2) in vec3 normal;

out vec2 texture_position_sampler;
out vec3 normal_sampler;
out vec3 position_sampler; 

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(position, 1);
  texture_position_sampler = texture_position;
  normal_sampler = mat3(transpose(inverse(model))) * normal;
  position_sampler = vec3(model * vec4(position, 1)); 
}
