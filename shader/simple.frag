#version 330

// uniform sampler2D texture;
// uniform float opacity;
// in vec2 texcoord;
in vec2 texture_position_sampler;
in vec3 normal_sampler;
in vec3 position_sampler;

uniform sampler2D texture;

uniform vec4 ambient_light;
uniform vec3 camera_view_position;

void main() {
  vec4 frag_color = texture2D(texture, vec2(texture_position_sampler.x,
                                            1 - texture_position_sampler.y));
  vec3 ambient_light_color = vec3(ambient_light.xyz * ambient_light.w);

  // Diffuse Light
  vec3 light_position = vec3(-5.0, 1.0, 1.0);
  vec3 normal = normalize(normal_sampler);
  vec3 light_direction = normalize(light_position - position_sampler);
  // float diff = max(dot(normal, light_direction), 0.0);
  vec3 diffuse = max(dot(normal, light_direction), 0.0) * ambient_light.xyz;

  // Specular Light
  float specular_strength = 1;
  vec3 view_direction = normalize(camera_view_position - position_sampler);
  vec3 reflect_direction = reflect(-light_direction, normal);
  float spec = pow(max(dot(view_direction, reflect_direction), 0.0), 32);
  vec3 specular = specular_strength * spec * ambient_light.xyz;

  gl_FragColor =
      vec4(ambient_light_color + (diffuse * specular) + diffuse, 1.0f) *
      frag_color;
}
