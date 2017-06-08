#include "cef-ui-shader.h"
namespace render {
namespace cef {

GLfloat CefUIShader::positions_[] = {-1.0f, -1.0f, 0.0f,  1.0f, 1.0f, -1.0f,
                                     0.0f,  1.0f,  -1.0f, 1.0f, 0.0f, 1.0f,
                                     1.0f,  1.0f,  0.0f,  1.0f};
GLushort CefUIShader::indices_[] = {0, 1, 2, 3};
GLfloat CefUIShader::texcoords_[] = {0.0f, 0.0f, 1.0f, 0.0f,
                                     0.0f, 1.0f, 1.0f, 1.0f};

void CefUIShader::OnInit() {
  CreateVertexBuffer(&positions_buffer_, GL_STATIC_DRAW, positions_, 16);
  CreateElementBuffer(&indices_buffer_, GL_STATIC_DRAW, indices_, 4);
  CreateVertexBuffer(&texcoords_buffer_, GL_DYNAMIC_DRAW, texcoords_, 8);

  AddShader("shader/cef-ui-vert.glsl", GLShader::kVertexShader);
  AddShader("shader/cef-ui-frag.glsl", GLShader::kFragmentShader);

  // Update with a fictive values
  // Update(1920, 1080);
}

void CefUIShader::OnDestroy() {}

void CefUIShader::PostCompile() {
  positions_attrib_.Locate(program_, "coords");
  texcoords_attrib_.Locate(program_, "texcoords");
}

void CefUIShader::Update(i32 width, i32 height) {
  set_projection(core::Ortho::Calculate(0.0, width, -height, 0.0, -10000, 10000));
  set_view(core::Matrix4f::Idenitity());
  texture_.Generate(width, height);
}

void CefUIShader::PassAndEnableAttributes() {
  positions_buffer_.Bind();
  positions_attrib_.PassVertexPointer(positions_buffer_, 4);
  positions_attrib_.Enable();

  texcoords_buffer_.Bind();
  texcoords_attrib_.PassVertexPointer(texcoords_buffer_, 2);
  texcoords_attrib_.Enable();
}

void CefUIShader::DisableAttributes() {
  positions_attrib_.Disable();
  texcoords_attrib_.Disable();
}

void CefUIShader::Render() {
  glEnable(GL_DEPTH_TEST);
  GLTexture::EnableBlending();
  
  program_.Use();
  texture_.Bind(); 
  PassProjectionViewUnifroms();
  PassAndEnableAttributes();
  indices_buffer_.Bind();

  glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)0);
  DisableAttributes(); 
}

}  // namespace cef
}  // namespace render
