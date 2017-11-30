#include "simple-shape.h"

namespace app {

bool SimpleShape::Init() {
  // Clear all past GL errors. Those should have been handled somewhere else.
  while (glGetError() != GL_NO_ERROR)
    ;

  glGenVertexArrays(1, &vertex_array_id_);
  glBindVertexArray(vertex_array_id_);

  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    log << util::kLogDateTime << ": " << __FILE__ << ":" << __LINE__ << ": "
        << (const char*)gluErrorString(err) << "\n";
    return false;
  }

  GLfloat positions[] = {-1.0f, -1.0f, 0.0f, -1.0f, 1.0f,  0.0f,
                         1.0f,  1.0f,  0.0f, 1.0f,  -1.0f, 0.0f};
  GLushort indices[] = {0, 1, 2, 0, 2, 3};
  GLfloat texture_positions[] = {0.0f, 1.0f, 0.0f, 0.0f,
                                 1.0f, 0.0f, 1.0f, 1.0f};
  GLfloat normals[] = {0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
                       0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f};

  render::CreateVertexBuffer(&positions_buffer_, GL_STATIC_DRAW, positions,
                             sizeof(positions) / sizeof(GLfloat));
  render::CreateVertexBuffer(&texture_positions_buffer_, GL_DYNAMIC_DRAW,
                             texture_positions,
                             sizeof(texture_positions) / sizeof(GLfloat));
  render::CreateVertexBuffer(&normals_buffer_, GL_STATIC_DRAW, normals,
                             sizeof(normals) / sizeof(GLfloat));

  render::CreateElementBuffer(&indices_buffer_, GL_STATIC_DRAW, indices,
                              sizeof(indices) / sizeof(GLushort));
  shader_.Init();
  if (!shader_.AddShader("shader/simple.vert",
                         render::GLShader::kVertexShader)) {
    return false;
  }

  if (!shader_.AddShader("shader/simple.frag",
                         render::GLShader::kFragmentShader)) {
    return false;
  }

  shader_.Compile();  // NOTE: This may be renamed Link in future.

  // Note, shaders model_view_projection unifrom is default to location 0.
  // Use shader_.ReallocateModelViewProjectionUnifrom to change.
  positions_attrib_.BindLocation(shader_.get_program(), 0, "position");
  texture_positions_attrib_.BindLocation(shader_.get_program(), 1,
                                         "texture_position");
  normals_attrib_.BindLocation(shader_.get_program(), 2, "normal");

  // Pass and enable attributes
  // Vertex attribute pointers and index buffers are part of Vertex Array Object
  // state, so they only have to be called once on the beginning / or when
  // they're modified.
  positions_buffer_.Bind();
  positions_attrib_.Enable();
  positions_attrib_.PassVertexPointer(positions_buffer_, 3);
  texture_positions_buffer_.Bind();
  texture_positions_attrib_.Enable();
  texture_positions_attrib_.PassVertexPointer(positions_buffer_, 2);
  normals_buffer_.Bind();
  normals_attrib_.Enable();
  normals_attrib_.PassVertexPointer(positions_buffer_, 3);
  
  indices_buffer_.Bind();

  image_.Load("textures/arid_01_diffuse.dds");
  texture_.Init(image_);
  texture_.Bind(); 

  glBindVertexArray(0);
  render::GLBufferBase::UnbindAll();
  return true;
}

void SimpleShape::Render() {
  glBindVertexArray(vertex_array_id_);
  shader_.get_program_unsafe()->Use();

  // Pass changed uniform
  if (model_view_projection_changed_) {
    // TODO(ppofuk): Recalculate model_view_project
    glm::mat4x4 model = glm::translate(translate_) *
                        glm::eulerAngleXZ(glm::radians(euler_xz_.x),
                                          glm::radians(euler_xz_.y)) *
                        glm::scale(scale_);


    glm::mat4x4 view =
        glm::lookAt(camera_position_, camera_target_, up_vector_);

    glm::mat4 projection = glm::perspective(glm::radians(fov_), aspect_ratio_,
                                            0.0001f, 1000000.0f);

    shader_.model_uniform_pass(model);
    shader_.view_uniform_pass(view);
    shader_.projection_uniform_pass(projection);

    model_view_projection_changed_ = false;
  }

  // Bind textures
  texture_.Bind(); 
  
  // Draw elements
  glDrawElements(GL_TRIANGLES, indices_buffer_.size(), GL_UNSIGNED_SHORT,
                 nullptr);  // Last is offset

  // Unbind Vertex Array Object
  glBindVertexArray(0);
  render::GLBufferBase::UnbindAll();
}

void SimpleShape::Destroy() {
  glBindVertexArray(vertex_array_id_);
  positions_attrib_.Disable();
  positions_buffer_.Destroy();
  indices_buffer_.Destroy();
  image_.Destroy();
  texture_.Destroy(); 
  shader_.Destroy();

  glDisableVertexAttribArray(vertex_array_id_);
  glBindVertexArray(0);
  render::GLBufferBase::UnbindAll();
}

void SimpleShape::Gui() {
  ImGui::Begin("Simple Shape");
  ImGui::Text("Model parameters (model matrix):");

  if (ImGui::SliderFloat3("Translatation", glm::value_ptr(translate_), -100.0f,
                          100.0f)) {
    model_view_projection_changed_ = true;
  }

  if (ImGui::SliderFloat2("Scale", glm::value_ptr(scale_), 1.0, 100.0f)) {
    model_view_projection_changed_ = true;
  }

  if (ImGui::SliderFloat2("Euler angles (x, z)", glm::value_ptr(euler_xz_),
                          0.0f, 360.0f)) {
    model_view_projection_changed_ = true;
  }

  ImGui::Text("Camera parameters (view matrix):");
  if (ImGui::SliderFloat3("Camera position", glm::value_ptr(camera_position_),
                          -100.0f, 100.0f)) {
    model_view_projection_changed_ = true;
  }

  if (ImGui::SliderFloat3("Camera target", glm::value_ptr(camera_target_),
                          -100.0f, 100.0f)) {
    model_view_projection_changed_ = true;
  }

  if (ImGui::SliderFloat3("Up vector", glm::value_ptr(up_vector_), -1.0f,
                          1.0f)) {
    model_view_projection_changed_ = true;
  }

  ImGui::Text("Add to camera position and target:");
  static glm::vec3 add_to_camera = glm::vec3(0.0f);

  if (ImGui::SliderFloat3("Add to camera", glm::value_ptr(add_to_camera),
                          -100.0f, 100.0f)) {
    model_view_projection_changed_ = true;
  }

  if (ImGui::Button("Apply")) {
    camera_position_ += add_to_camera;
    camera_target_ += add_to_camera;
    add_to_camera = glm::vec3(0.0f);
    model_view_projection_changed_ = true;
  }

  ImGui::Text("Projection parameters:");
  if (ImGui::SliderFloat("Field of View", &fov_, 0.1f, 359.0f)) {
    model_view_projection_changed_ = true;
  }

  ImGui::Text("Aspect ratio: %.3f", aspect_ratio_);
  ImGui::End();
}

}  // namespace app
