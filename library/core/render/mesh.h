#ifndef CORE_RENDER_MESH_H_
#define CORE_RENDER_MESH_H_

#include "core/make-four-cc-inl.h"
#include "core/serialize/serialize.h"
#include "core/basic-types.h"
#include "core/logger.h"
#include "glm/glm.hpp"
#include <vector>
#include <cstdio>

namespace render {

class Mesh : public serialize::Serializable<Mesh> {
 public:
  bool Load(const char* filename);

  inline const std::vector<glm::vec3>& vertices() { return vertices_; }
  inline const std::vector<glm::vec3>& normals() { return normals_; }
  inline const std::vector<glm::vec2>& texcoords() { return texcoords_; }
  inline const std::vector<u16>& indices() { return indices_; }
  inline std::vector<glm::vec3>* mutable_vertices() { return &vertices_; }
  inline std::vector<glm::vec3>* mutable_normals() { return &normals_; }
  inline std::vector<glm::vec2>* mutable_texcoords() { return &texcoords_; }
  inline std::vector<u16>* mutable_indices() { return &indices_; }
  inline void set_vertices(const std::vector<glm::vec3>& vertices) {
    vertices_ = vertices;
  }
  inline void set_normals(const std::vector<glm::vec3>& normals) {
    normals_ = normals;
  }
  inline void set_indices(const std::vector<u16>& indices) {
    indices_ = indices;
  }
  inline void set_texcoords(const std::vector<glm::vec2>& texcoords) {
    texcoords_ = texcoords;
  }


 private:
  mutable std::vector<glm::vec3> vertices_;
  mutable std::vector<glm::vec3> normals_;
  mutable std::vector<glm::vec2> texcoords_;
  mutable std::vector<u16> indices_;
};

}  // render

#endif  // CORE_RENDER_MESH_H_
