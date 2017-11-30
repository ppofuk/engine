#ifndef APP_MESH_CONVERT_SPHERE_H_
#define APP_MESH_CONVERT_SPHERE_H_

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

class Sphere : public serialize::Serializable<Sphere> {
 public:
  bool Generate(u32 rings = 24, u32 sectors = 48);
  bool is_generated() const { return is_generated_; }

  inline bool Save(const char* filename) {
    return Serialize(filename, vertices_, normals_, texcoords_, indices_);
  }

  const std::vector<glm::vec3>& vertices() { return vertices_; }
  const std::vector<glm::vec3>& normals() { return normals_; }
  const std::vector<glm::vec2>& texcoords() { return texcoords_; }
  const std::vector<u16>& indices() { return indices_; }

 private:
  bool is_generated_ = false;
  std::vector<glm::vec3> vertices_;
  std::vector<glm::vec3> normals_;
  std::vector<glm::vec2> texcoords_;
  std::vector<u16> indices_;
};

}  // namespace mesh

#endif  // APP_MESH_CONVERT_SPHERE_H_
