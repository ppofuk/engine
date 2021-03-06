#ifndef APP_MESH_CONVERT_PLANE_H_
#define APP_MESH_CONVERT_PLANE_H_

#define GLM_ENABLE_EXPERIMENTAL

#include "core/basic-types.h"
#include "core/logger.h"
#include "core/serialize/serialize.h"
#include "glm/glm.hpp"
#include "glm/gtx/normal.hpp"
#include "glm/gtc/constants.hpp"
#include <vector>
#include <cmath>

namespace mesh {

class Plane : public serialize::Serializable<Plane> {
 public:
  bool is_generated() const { return is_generated_; }
  inline bool Save(const char* filename) {
    return Serialize(filename, vertices_, normals_, texcoords_, indices_);
  }

  std::vector<glm::vec3> vertices() { return vertices_; }
  const std::vector<glm::vec3>& normals() { return normals_; }
  const std::vector<glm::vec2>& texcoords() { return texcoords_; }
  const std::vector<u16>& indices() { return indices_; }

 private:
  bool is_generated_ = true;
  std::vector<glm::vec3> vertices_{
      glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec3(-1.0f, 1.0f, 0.0f),
      glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0f, -1.0f, 0.0f)};
  std::vector<glm::vec3> normals_{
      glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f),
      glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, -1.0f)};
  std::vector<glm::vec2> texcoords_{
      glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(1.0f, 0.0f),
      glm::vec2(1.0f, 1.0f)};
  std::vector<u16> indices_{0, 1, 2, 0, 2, 3};
};

}  // namespace mesh

#endif  // APP_MESH_CONVERT_PLANE_H_
