#include "window-sdl.h"

namespace core {

void WindowSDL::DumpSDLLog() {
  log << util::kLogDateTime << ": " << SDL_GetError() << "\n";
}

bool WindowSDL::Init(const char* title, i32 x, i32 y, i32 width, i32 height) {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    DumpSDLLog();
    return false;
  }

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, gl_context_options_.double_buffer);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, gl_context_options_.depth_size);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, gl_context_options_.stencil_size);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                      gl_context_options_.context_major_version);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                      gl_context_options_.context_minor_version);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,
                      gl_context_options_.multisample_buffers);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,
                      gl_context_options_.multisample_samples);


  if (SDL_GetCurrentDisplayMode(0, &display_mode_) != 0) {
    DumpSDLLog();
    return false;
  }

  window_ = SDL_CreateWindow(title, x, y, width, height,
                             SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
  if (window_ == nullptr) {
    DumpSDLLog();
    return false;
  }
  UpdateSize();

  gl_context_ = SDL_GL_CreateContext(window_);
  if (gl_context_ == nullptr) {
    DumpSDLLog();
    return false;
  }

  // TODO: glad loader
  if (!gladLoadGL()) {
    log << util::kLogDateTime << ": Failed to initialize OpenGL context\n";
  }

  char* gl_version = (char*)glGetString(GL_VERSION);
  char* gl_vendor = (char*)glGetString(GL_VENDOR);
  char* gl_renderer = (char*)glGetString(GL_RENDERER);
  log << util::kLogDateTime << ": "
      << "GL Context created\n"
      << util::kLogDateTime << ": "
      << "GL version: " << gl_version << "\n"
      << util::kLogDateTime << ": "
      << "GL vendor: " << gl_vendor << "\n"
      << util::kLogDateTime << ": "
      << "GL renderer: " << gl_renderer << "\n";

  is_init_ = true;
  return true;
}

void WindowSDL::Destroy() {
  if (gl_context_) {
    SDL_GL_DeleteContext(gl_context_);
    gl_context_ = nullptr;
  }

  if (window_) {
    SDL_DestroyWindow(window_);
    window_ = nullptr;
  }

  SDL_Quit();
  is_init_ = false;
}

void WindowSDL::Prerender() {
  // Nothing to do here.
}

void WindowSDL::Postrender() {
  // Swap chain
  // TODO(ppofuk): check is_init_ maybe?
  SDL_GL_SwapWindow(window_);
}

void WindowSDL::UpdateSize() {
  SDL_GetWindowSize(window_, &width_, &height_);
}

bool WindowSDL::CheckForEvents() {
  has_event_ = SDL_PollEvent(&event_) == 1;
  if (has_event_) {
    if (event_.type == SDL_WINDOWEVENT) {
      if (event_.window.event == SDL_WINDOWEVENT_RESIZED ||
          event_.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
          event_.window.event == SDL_WINDOWEVENT_EXPOSED) {
        UpdateSize();
      }
    }
  }

  return has_event_;
}

void WindowSDL::Title(const char* title) {
  SDL_SetWindowTitle(window_, title);
}

void WindowSDL::Show(int) {
  SDL_ShowWindow(window_);
}

void WindowSDL::Hide() {
  SDL_HideWindow(window_);
}

}  // namespace core
