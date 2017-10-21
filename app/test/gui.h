#ifndef APP_TOOLKIT_GUI_H_
#define APP_TOOLKIT_GUI_H_

#include "imgui/imgui.h"
#include "core/window-sdl.h"

namespace toolkit {


class Gui {
 public:
  IMGUI_API bool Init(core::WindowSDL* window, bool install_callbacks = false);
  IMGUI_API void Shutdown();
  IMGUI_API void NewFrame();
  IMGUI_API void InvalidateDeviceObjects();
  IMGUI_API bool CreateDeviceObjects();
  IMGUI_API bool ProcessEvent(SDL_Event* event);

  static void RenderDrawLists(ImDrawData* draw_data);
  static const char* GetClipboardText(void*);
  static void SetClipboardText(void*, const char* text);

  static i32 shader_handle;
  static i32 attrib_location_tex;
  static i32 attrib_location_proj_matrix;
  static u32 vao_handle;
  static u32 vbo_handle;
  static u32 elements_handle;

 private:
  
  void CreateFontTexture();

  // Set shader_handle and etc. to shader_handle_. Fucking imgui. 
  void SetGlobalHandles(); 

  f64 time_ = 0.0f;
  bool mouse_pressed_[3] = {false, false, false};
  f32 mouse_wheel_ = 0.0f;
  GLuint font_texture_ = 0;
  i32 shader_handle_ = 0;
  i32 vert_handle_ = 0;
  i32 frag_handle_ = 0;
  i32 attrib_location_tex_ = 0;
  i32 attrib_location_proj_matrix_ = 0;
  i32 attrib_location_position_ = 0;
  i32 attrib_location_uv_ = 0;
  i32 attrib_location_color_ = 0;
  u32 vbo_handle_ = 0;
  u32 vao_handle_ = 0;
  u32 elements_handle_ = 0;
  bool is_init_ = false;
  
  core::WindowSDL* window_ = nullptr; 
};


}  // namespace toolkit


#endif  // APP_TOOLKIT_GUI_H_
