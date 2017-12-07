#ifndef SIMPLE_SHAPE_H_
#define SIMPLE_SHAPE_H_

#include "core/render/gl-buffer.h"
#include "core/render/gl-shader.h"
#include "core/render/gl-attribute.h"
#include "core/render/gl-texture.h"
#include "core/render/mesh.h"
#include "core/render/camera.h"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"

#include "gui.h"

namespace app {

class SimpleShape : private util::HasLog {
 public:
  // Initalize buffers and shaders.
  bool Init(const char* mesh_filename = "sphere.bin");

  // Render simple shape.
  void Render();

  // Destroy method. Should be called for cleanup.
  void Destroy();

  // Create Imgui window for this object. Should be called before
  // rendering.
  void Gui();

  // Change aspect ratio for projection matrix perspective.
  // Forces recalculation of model view projection matrix.
  inline void set_aspect_ratio(f32 aspect_ratio) {
    aspect_ratio_ = aspect_ratio;
    model_view_projection_changed_ = true;
  };

  render::GLShader* shader() { return &shader_; }
  void set_camera(render::Camera* camera) { camera_ = camera; }
  render::Camera* mutable_camera() { return camera_; }
  void ForceUpdateModelViewProjection(); 

 private:
  render::Mesh mesh_;
  render::GLBuffer<GLfloat> positions_buffer_;
  render::GLBuffer<GLushort> indices_buffer_;
  render::GLBuffer<GLfloat> texture_positions_buffer_;
  render::GLBuffer<GLfloat> normals_buffer_;
  render::GLAttribute<render::GLBuffer<GLfloat> > positions_attrib_;
  render::GLAttribute<render::GLBuffer<GLfloat> > texture_positions_attrib_;
  render::GLAttribute<render::GLBuffer<GLfloat> > normals_attrib_;
  render::GLShader shader_;
  GLuint vertex_array_id_ =
      0;  // TODO(ppofuk): create abstraction for Vertex Array Object
  
  render::GLTexture texture_;
  render::DDSImage image_;

  glm::vec3 translate_ = glm::vec3(0.0f);
  glm::vec3 scale_ = glm::vec3(1.0f);
  glm::vec3 euler_xz_ = glm::vec3(0.0f);

  glm::vec3 camera_position_ = glm::vec3(0.0f, 0.0f, 10.0f);
  glm::vec3 camera_target_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 up_vector_ = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 camera_euler_ = glm::vec3(0.0f);

  mutable render::Camera* camera_ = nullptr;

  f32 fov_ = 45.0;
  f32 aspect_ratio_ = 4.0f / 3.0f;

  bool model_view_projection_changed_ = true;
};

}  // namespace app

#endif /* SIMPLE_SHAPE_H_ */
