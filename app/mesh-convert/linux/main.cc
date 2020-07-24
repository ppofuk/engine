#define GLM_ENABLE_EXPERIMENTAL

#include "core/render/gl-types.h"
#include "core/logger.h"
#include "glm/glm.hpp"
#include "glm/gtx/normal.hpp"
#include "../sphere.h"
#include "../plane.h"

#include "core/render/mesh.h"
#include <iostream>

int main(int argc, char* argv[]) {
  // mesh::Plane plane;
  mesh::Sphere sphere;

  // plane.Save("plane.bin");
  sphere.Generate();
  sphere.Save("sphere.bin");

  render::Mesh mesh;
  mesh.Load("plane.bin");

  std::cout << "mesh.vertices = {";
  for (const auto& i : mesh.vertices()) {
    std::cout << "{" << i.x << ", " << i.y << ", " << i.z << "} ";
  }
  std::cout << "}" << std::endl;

  std::cout << "mesh.normals = {";
  for (const auto& i : mesh.normals()) {
    std::cout << "{" << i.x << ", " << i.y << ", " << i.z << "} ";
  }
  std::cout << "}" << std::endl;

  std::cout << "mesh.texcoords = {";
  for (const auto& i : mesh.texcoords()) {
    std::cout << "{" << i.x << ", " << i.y << "} ";
  }
  std::cout << "}" << std::endl;

  std::cout << "mesh.indices = {";
  for (const auto& i : mesh.indices()) {
    std::cout << "{" << i << "} ";
  }
  std::cout << "}" << std::endl;
  return 0;
}
