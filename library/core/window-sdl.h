#ifndef CORE_WINDOW_SDL_H_
#define CORE_WINDOW_SDL_H_

#include "glad/glad.h"
#include "core/basic-types.h"
#include "core/logger.h"

#include <SDL.h>
#include "core/render/gl-context-options.h"

namespace core {

class WindowSDL : public util::HasLog {
 public:
  bool Init(const char* title,
            i32 x = 0,
            i32 y = 0,
            i32 width = 800,
            i32 height = 600);

  void Destroy();

  // Should be called after rendering is complete.
  void Postrender();

  // Should be called before rendering.
  void Prerender();

  // Returns true if there is a pending event from internal event queue.
  // Event is dequeued. The dequeued event can be accessed with |event()|. 
  bool CheckForEvents();

  // Maps a window to screen. Note, that this is done in |Init|.
  // show_param is ignored (it's just for compatibility with win32 version).
  void Show(int show_param = 0);

  // Unmaps a window from screen.
  void Hide();

  // Sets window title. In X it's called store name.
  void Title(const char* title);

  inline GLContextOptions gl_context_options() { return gl_context_options_; }
  inline void set_gl_context_options_preinit(
      GLContextOptions gl_context_options) {
    if (!is_init_) {
      gl_context_options_ = gl_context_options;
    }
  }

  SDL_Window* window() { return window_; }
  SDL_DisplayMode display_mode() const { return display_mode_; }
  
  bool is_init() const { return is_init_; }

  bool has_event() const { return has_event_; }
  inline SDL_Event* event() { return &event_; }

  i32 width() const { return width_; }
  i32 height() const { return height_; }

 private:
  void DumpSDLLog();
  // Update width_ and height_ to match window size.
  // This is also called from |CheckForEvents|.
  void UpdateSize(); 

  i32 width_ = 0;
  i32 height_ = 0; 
  
  bool is_init_ = false;
  SDL_Window* window_ = nullptr;
  SDL_DisplayMode display_mode_;
  SDL_GLContext gl_context_;
  GLContextOptions gl_context_options_;
  SDL_Event event_;
  bool has_event_ = false;
};

}  // namespace core

#endif  // CORE_WINDOW_SDL_H_
