#include "sphere.h"

namespace mesh {

bool Sphere::Generate(u32 rings, u32 sectors) {
  vertices_.clear();
  texcoords_.clear();
  normals_.clear();

  f32 const R = 1.0f / (f32)(rings - 1);
  f32 const S = 1.0f / (f32)(sectors - 1);
  f32 pi = 3.14159265358979323846f;

  for (u32 r = 0; r < rings; ++r) {
    for (u32 s = 0; s < sectors; ++s) {
      f32 const y = std::sin(-pi / 2.0f + pi * r * R);
      f32 const x = std::cos(2 * pi * s * S) * std::sin(pi * r * R);
      f32 const z = std::sin(2 * pi * s * S) * std::sin(pi * r * R);

      glm::vec2 texcoord = glm::vec2(s * S, r * R);
      glm::vec3 vertex = glm::vec3(x, y, z);

      vertices_.push_back(vertex);
      texcoords_.push_back(texcoord);
    }
  }

  indices_.resize(rings * sectors * 6);
  auto i = indices_.begin();
  for (u32 r = 0; r < rings - 1; ++r) {
    for (u32 s = 0; s < sectors - 1; ++s) {
      *i++ = r * sectors + s;
      *i++ = r * sectors + (s + 1);
      *i++ = (r + 1) * sectors + (s + 1);

      *i++ = (r + 1) * sectors + (s + 1);
      *i++ = (r + 1) * sectors + s;
      *i++ = r * sectors + s;
    }
  }

  normals_.resize(vertices_.size(), glm::vec3(0.0f));
  for (u32 i = 0; i < indices_.size(); i += 3) {
    glm::vec3 normal =
        glm::triangleNormal(vertices_[indices_[i]], vertices_[indices_[i + 1]],
                            vertices_[indices_[i + 2]]);
    normals_[indices_[i]] = glm::normalize(normals_[indices_[i]] + normal);
    normals_[indices_[i + 1]] =
        glm::normalize(normals_[indices_[i + 1]] + normal);
    normals_[indices_[i + 2]] =
        glm::normalize(normals_[indices_[i + 2]] + normal);
  }

  for (const glm::vec3& vertex : vertices_) {
    util::Log << util::kLogDateTime << ": vertex(" << vertex.x << ", "
              << vertex.y << ", " << vertex.z << ")\n";
  }

  util::Log << util::kLogDateTime << ": indices (";
  for (const u32& index : indices_) {
    util::Log << (i32)index << ", ";
  }
  util::Log << ")\n";

  for (const glm::vec3& normal : normals_) {
    util::Log << util::kLogDateTime << ": normal(" << normal.x << ", "
              << normal.y << ", " << normal.z << ")\n";
  }

  is_generated_ = true;
  return true;
}

}  // namespace mesh
