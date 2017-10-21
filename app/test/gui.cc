#include "gui.h"

namespace toolkit {

i32 Gui::shader_handle = 0;
i32 Gui::attrib_location_tex = 0;
i32 Gui::attrib_location_proj_matrix = 0;
u32 Gui::vao_handle = 0;
u32 Gui::vbo_handle = 0;
u32 Gui::elements_handle = 0;

bool Gui::Init(core::WindowSDL* window, bool install_callbacks) {
  window_ = window;
  ImGuiIO& io = ImGui::GetIO();
  io.KeyMap[ImGuiKey_Tab] = SDLK_TAB;  // Keyboard mapping. ImGui will use those
                                       // indices to peek into the io.KeyDown[]
                                       // array.
  io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
  io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
  io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
  io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
  io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
  io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
  io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
  io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
  io.KeyMap[ImGuiKey_Delete] = SDLK_DELETE;
  io.KeyMap[ImGuiKey_Backspace] = SDLK_BACKSPACE;
  io.KeyMap[ImGuiKey_Enter] = SDLK_RETURN;
  io.KeyMap[ImGuiKey_Escape] = SDLK_ESCAPE;
  io.KeyMap[ImGuiKey_A] = SDLK_a;
  io.KeyMap[ImGuiKey_C] = SDLK_c;
  io.KeyMap[ImGuiKey_V] = SDLK_v;
  io.KeyMap[ImGuiKey_X] = SDLK_x;
  io.KeyMap[ImGuiKey_Y] = SDLK_y;
  io.KeyMap[ImGuiKey_Z] = SDLK_z;

  io.RenderDrawListsFn = RenderDrawLists;
  io.SetClipboardTextFn = SetClipboardText;
  io.GetClipboardTextFn = GetClipboardText;
  io.ClipboardUserData = nullptr;

  // TODO(ppofuk): set io.ImeWindowHandle for win32

  is_init_ = true;
  return true;
}

bool Gui::CreateDeviceObjects() {
  // Backup GL state
  GLint last_texture, last_array_buffer, last_vertex_array;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);

  const GLchar* vertex_shader =
      "#version 330\n"
      "uniform mat4 ProjMtx;\n"
      "in vec2 Position;\n"
      "in vec2 UV;\n"
      "in vec4 Color;\n"
      "out vec2 Frag_UV;\n"
      "out vec4 Frag_Color;\n"
      "void main()\n"
      "{\n"
      "	Frag_UV = UV;\n"
      "	Frag_Color = Color;\n"
      "	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
      "}\n";

  const GLchar* fragment_shader =
      "#version 330\n"
      "uniform sampler2D Texture;\n"
      "in vec2 Frag_UV;\n"
      "in vec4 Frag_Color;\n"
      "out vec4 Out_Color;\n"
      "void main()\n"
      "{\n"
      "	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
      "}\n";

  shader_handle_ = glCreateProgram();
  vert_handle_ = glCreateShader(GL_VERTEX_SHADER);
  frag_handle_ = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(vert_handle_, 1, &vertex_shader, 0);
  glShaderSource(frag_handle_, 1, &fragment_shader, 0);
  glCompileShader(vert_handle_);
  glCompileShader(frag_handle_);
  glAttachShader(shader_handle_, vert_handle_);
  glAttachShader(shader_handle_, frag_handle_);
  glLinkProgram(shader_handle_);

  attrib_location_tex_ = glGetUniformLocation(shader_handle_, "Texture");
  attrib_location_proj_matrix_ =
      glGetUniformLocation(shader_handle_, "ProjMtx");
  attrib_location_position_ = glGetAttribLocation(shader_handle_, "Position");
  attrib_location_uv_ = glGetAttribLocation(shader_handle_, "UV");
  attrib_location_color_ = glGetAttribLocation(shader_handle_, "Color");

  glGenBuffers(1, &vbo_handle_);
  glGenBuffers(1, &elements_handle_);

  glGenVertexArrays(1, &vao_handle_);
  glBindVertexArray(vao_handle_);
  glBindBuffer(GL_ARRAY_BUFFER, vbo_handle_);
  glEnableVertexAttribArray(attrib_location_position_);
  glEnableVertexAttribArray(attrib_location_uv_);
  glEnableVertexAttribArray(attrib_location_color_);

#define OFFSETOF(TYPE, ELEMENT) ((size_t) & (((TYPE*)0)->ELEMENT))
  glVertexAttribPointer(attrib_location_position_, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
  glVertexAttribPointer(attrib_location_uv_, 2, GL_FLOAT, GL_FALSE,
                        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
  glVertexAttribPointer(attrib_location_color_, 4, GL_UNSIGNED_BYTE, GL_TRUE,
                        sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
#undef OFFSETOF

  CreateFontTexture();

  // Restore states
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBindVertexArray(last_vertex_array);

  return true;
}

void Gui::CreateFontTexture() {
  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  unsigned char* pixels;
  int width, height;
  io.Fonts->GetTexDataAsRGBA32(
      &pixels, &width, &height);  // Load as RGBA 32-bits for OpenGL3 demo
                                  // because it is more likely to be compatible
                                  // with user's existing shader.

  // Upload texture to graphics system
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  glGenTextures(1, &font_texture_);
  glBindTexture(GL_TEXTURE_2D, font_texture_);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, pixels);

  // Store our identifier
  io.Fonts->TexID = (void*)(intptr_t)font_texture_;

  // Restore state
  glBindTexture(GL_TEXTURE_2D, last_texture);
}

void Gui::InvalidateDeviceObjects() {
  if (vao_handle_)
    glDeleteVertexArrays(1, &vao_handle_);
  if (vbo_handle_)
    glDeleteBuffers(1, &vbo_handle_);
  if (elements_handle_)
    glDeleteBuffers(1, &elements_handle_);
  vao_handle_ = vbo_handle_ = elements_handle_ = 0;

  if (shader_handle_ && vert_handle_)
    glDetachShader(shader_handle_, vert_handle_);
  if (vert_handle_)
    glDeleteShader(vert_handle_);
  vert_handle_ = 0;

  if (shader_handle_ && frag_handle_)
    glDetachShader(shader_handle_, frag_handle_);
  if (frag_handle_)
    glDeleteShader(frag_handle_);
  frag_handle_ = 0;

  if (shader_handle_)
    glDeleteProgram(shader_handle_);
  shader_handle_ = 0;

  if (font_texture_) {
    glDeleteTextures(1, &font_texture_);
    ImGui::GetIO().Fonts->TexID = 0;
    font_texture_ = 0;
  }
}

void Gui::Shutdown() {
  InvalidateDeviceObjects();
  window_ = nullptr;
  ImGui::Shutdown(); 
}

void Gui::NewFrame() {
  SDL_Window* window = window_->window();
  SetGlobalHandles();

  if (!font_texture_)
    CreateDeviceObjects();

  ImGuiIO& io = ImGui::GetIO();

  // Setup display size (every frame to accommodate for window resizing)
  int w, h;
  int display_w, display_h;
  SDL_GetWindowSize(window, &w, &h);
  SDL_GL_GetDrawableSize(window, &display_w, &display_h);
  io.DisplaySize = ImVec2((float)w, (float)h);
  io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0,
                                      h > 0 ? ((float)display_h / h) : 0);

  // Setup time step
  Uint32 time = SDL_GetTicks();
  double current_time = time / 1000.0;
  io.DeltaTime =
      time_ > 0.0 ? (float)(current_time - time_) : (float)(1.0f / 60.0f);
  time_ = current_time;

  // Setup inputs
  // (we already got mouse wheel, keyboard keys & characters from
  // SDL_PollEvent())
  int mx, my;
  Uint32 mouseMask = SDL_GetMouseState(&mx, &my);
  if (SDL_GetWindowFlags(window) & SDL_WINDOW_MOUSE_FOCUS)
    io.MousePos = ImVec2((float)mx, (float)my);  // Mouse position, in pixels
                                                 // (set to -1,-1 if no mouse /
                                                 // on another screen, etc.)
  else
    io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX);

  io.MouseDown[0] = mouse_pressed_[0] ||
                    (mouseMask & SDL_BUTTON(SDL_BUTTON_LEFT)) !=
                        0;  // If a mouse press event came, always pass it as
                            // "mouse held this frame", so we don't miss
                            // click-release events that are shorter than 1
                            // frame.
  io.MouseDown[1] =
      mouse_pressed_[1] || (mouseMask & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0;
  io.MouseDown[2] =
      mouse_pressed_[2] || (mouseMask & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0;
  mouse_pressed_[0] = mouse_pressed_[1] = mouse_pressed_[2] = false;

  io.MouseWheel = mouse_wheel_;
  mouse_wheel_ = 0.0f;

  // Hide OS mouse cursor if ImGui is drawing it
  SDL_ShowCursor(io.MouseDrawCursor ? 0 : 1);

  // Start the frame
  ImGui::NewFrame();
}

bool Gui::ProcessEvent(SDL_Event* event) {
  ImGuiIO& io = ImGui::GetIO();
  switch (event->type) {
    case SDL_MOUSEWHEEL: {
      if (event->wheel.y > 0)
        mouse_wheel_ = 1;
      if (event->wheel.y < 0)
        mouse_wheel_ = -1;
      return true;
    }
    case SDL_MOUSEBUTTONDOWN: {
      if (event->button.button == SDL_BUTTON_LEFT)
        mouse_pressed_[0] = true;
      if (event->button.button == SDL_BUTTON_RIGHT)
        mouse_pressed_[1] = true;
      if (event->button.button == SDL_BUTTON_MIDDLE)
        mouse_pressed_[2] = true;
      return true;
    }
    case SDL_TEXTINPUT: {
      io.AddInputCharactersUTF8(event->text.text);
      return true;
    }
    case SDL_KEYDOWN:
    case SDL_KEYUP: {
      int key = event->key.keysym.sym & ~SDLK_SCANCODE_MASK;
      io.KeysDown[key] = (event->type == SDL_KEYDOWN);
      io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
      io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
      io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
      io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);
      return true;
    }
  }
  return false;
}

const char* Gui::GetClipboardText(void*) {
  return SDL_GetClipboardText();
}

void Gui::SetClipboardText(void*, const char* text) {
  SDL_SetClipboardText(text);
}

void Gui::SetGlobalHandles() {
  Gui::shader_handle = shader_handle_;
  Gui::attrib_location_proj_matrix = attrib_location_proj_matrix_;
  Gui::attrib_location_tex = attrib_location_tex_;
  Gui::vao_handle = vao_handle_;
  Gui::vbo_handle = vbo_handle_;
  Gui::elements_handle = elements_handle_;
}

void Gui::RenderDrawLists(ImDrawData* draw_data) {
  // Avoid rendering when minimized, scale coordinates for retina displays
  // (screen coordinates != framebuffer coordinates)
  ImGuiIO& io = ImGui::GetIO();
  int fb_width = (int)(io.DisplaySize.x * io.DisplayFramebufferScale.x);
  int fb_height = (int)(io.DisplaySize.y * io.DisplayFramebufferScale.y);
  if (fb_width == 0 || fb_height == 0)
    return;
  draw_data->ScaleClipRects(io.DisplayFramebufferScale);

  // Backup GL state
  GLenum last_active_texture;
  glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
  glActiveTexture(GL_TEXTURE0);
  GLint last_program;
  glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
  GLint last_texture;
  glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  GLint last_sampler;
  glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
  GLint last_array_buffer;
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  GLint last_element_array_buffer;
  glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &last_element_array_buffer);
  GLint last_vertex_array;
  glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
  GLint last_polygon_mode[2];
  glGetIntegerv(GL_POLYGON_MODE, last_polygon_mode);
  GLint last_viewport[4];
  glGetIntegerv(GL_VIEWPORT, last_viewport);
  GLint last_scissor_box[4];
  glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
  GLenum last_blend_src_rgb;
  glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
  GLenum last_blend_dst_rgb;
  glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
  GLenum last_blend_src_alpha;
  glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
  GLenum last_blend_dst_alpha;
  glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
  GLenum last_blend_equation_rgb;
  glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
  GLenum last_blend_equation_alpha;
  glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
  GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
  GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
  GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
  GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

  // Setup render state: alpha-blending enabled, no face culling, no depth
  // testing, scissor enabled, polygon fill
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Setup viewport, orthographic projection matrix
  glViewport(0, 0, (GLsizei)fb_width, (GLsizei)fb_height);
  const float ortho_projection[4][4] = {
      {2.0f / io.DisplaySize.x, 0.0f, 0.0f, 0.0f},
      {0.0f, 2.0f / -io.DisplaySize.y, 0.0f, 0.0f},
      {0.0f, 0.0f, -1.0f, 0.0f},
      {-1.0f, 1.0f, 0.0f, 1.0f},
  };
  glUseProgram(Gui::shader_handle);
  glUniform1i(Gui::attrib_location_proj_matrix, 0);
  glUniformMatrix4fv(Gui::attrib_location_proj_matrix, 1, GL_FALSE,
                     &ortho_projection[0][0]);
  glBindVertexArray(Gui::vao_handle);
  glBindSampler(0, 0);  // Rely on combined texture/sampler state.

  for (int n = 0; n < draw_data->CmdListsCount; n++) {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    const ImDrawIdx* idx_buffer_offset = 0;

    glBindBuffer(GL_ARRAY_BUFFER, Gui::vbo_handle);
    glBufferData(GL_ARRAY_BUFFER,
                 (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert),
                 (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Gui::elements_handle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx),
                 (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
      const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
      if (pcmd->UserCallback) {
        pcmd->UserCallback(cmd_list, pcmd);
      } else {
        glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
        glScissor((int)pcmd->ClipRect.x, (int)(fb_height - pcmd->ClipRect.w),
                  (int)(pcmd->ClipRect.z - pcmd->ClipRect.x),
                  (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
        glDrawElements(
            GL_TRIANGLES, (GLsizei)pcmd->ElemCount,
            sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT,
            idx_buffer_offset);
      }
      idx_buffer_offset += pcmd->ElemCount;
    }
  }

  // Restore modified GL state
  glUseProgram(last_program);
  glBindTexture(GL_TEXTURE_2D, last_texture);
  glBindSampler(0, last_sampler);
  glActiveTexture(last_active_texture);
  glBindVertexArray(last_vertex_array);
  glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, last_element_array_buffer);
  glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
  glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb,
                      last_blend_src_alpha, last_blend_dst_alpha);
  if (last_enable_blend)
    glEnable(GL_BLEND);
  else
    glDisable(GL_BLEND);
  if (last_enable_cull_face)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);
  if (last_enable_depth_test)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);
  if (last_enable_scissor_test)
    glEnable(GL_SCISSOR_TEST);
  else
    glDisable(GL_SCISSOR_TEST);
  glPolygonMode(GL_FRONT_AND_BACK, last_polygon_mode[0]);
  glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2],
             (GLsizei)last_viewport[3]);
  glScissor(last_scissor_box[0], last_scissor_box[1],
            (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
}


}  // toolkit
