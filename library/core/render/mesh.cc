#include "core/render/mesh.h"
namespace render {

bool Mesh::Load(const char* filename) {
  return Deserialize(filename, vertices_, normals_, texcoords_, indices_);
}

}  // render
