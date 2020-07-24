#ifndef CORE_RENDER_GL_CONTEXT_OPTIONS_H_
#define CORE_RENDER_GL_CONTEXT_OPTIONS_H_

#include "core/basic-types.h"

namespace core {

struct GLContextOptions {
  u32 context_major_version = 3;
  u32 context_minor_version = 3;
  u32 depth_size = 24;
  u32 stencil_size = 8;
  u32 double_buffer = 1;
  u32 multisample_buffers = 1;
  u32 multisample_samples = 4; 
};

} // namespace core


#endif // CORE_RENDER_GL_CONTEXT_OPTIONS_H_
