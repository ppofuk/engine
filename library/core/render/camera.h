#ifndef CORE_RENDER_CAMERA_H_
#define CORE_RENDER_CAMERA_H_

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/transform.hpp"

namespace render {

class Camera {
 public:
  // Sets poistion_, target_, up_vector_ vectors and calls Recalculateviewmatrix
  void LookAt(const glm::vec3& position,
              const glm::vec3& target,
              const glm::vec3& up_vector = glm::vec3(0.0f, 1.0f, 0.0f));

  // Calculates a glm::mat4 view matrix with glm::lookAt call
  void RecalculateViewMatrix();

  // Applies euler tranformations on position_ vector and recalculates view
  // matrix.
  // euler_on_poistion_ vector components (x, y, z) are angles in degrees.
  // euler_on_position_ vector can be set with set_euler_on_position or with
  // AddRotationAroundPosition method
  void RecalculateViewMatrixWithRotation();

  // Adds |rotation| vector on |euler_on_position_| vector and calls
  // RecalculateviewMatrixWithRotation
  void ApplyRotationAroundPosition(const glm::vec3& rotation);

  // Multiplies a translate matrix position on view matrix.
  void Move(const glm::vec3& position);

  void Rotate(const glm::vec3& rotation);

  void RotateAround(const glm::vec3& rotation,
                    const glm::vec3& point_offset = glm::vec3(0.0f,
                                                              0.0f,
                                                              10.0f));

  // Adds vector |target_add| to |target_|. You should manualy recalculate
  // view matrix with |RecalculateViewMatrix| or
  // |RecalculateViewMatrixWithRotation|
  void target_add(const glm::vec3& target_add) { target_ += target_add; }

  // Adds vector |position_add| to |position_|. You should manualy recalculate
  // view matrix with |RecalculateViewMatrix| or
  // |RecalculateViewMatrixWithRotation|
  void position_add(const glm::vec3& position_add) {
    position_ += position_add;
  }

  glm::vec3 position() const {
    return glm::vec3(-view()[3].x, -view()[3].y, -view()[3].z);
  }
  
  inline const glm::vec3& target() const { return target_; }
  inline const glm::vec3& up_vector() const { return up_vector_; }
  inline const glm::vec3& euler_on_position() const {
    return euler_on_position_;
  }
  // Returns view matrix.
  inline const glm::mat4& view() const { return view_; }
  inline void set_euler_on_position(const glm::vec3& euler_on_position) {
    euler_on_position_ = euler_on_position;
  }

  // Returns |view_has_updated_| and resets it to false.
  bool HasViewUpdated();

 private:
  bool view_has_updated_ = false;
  glm::vec3 position_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 target_ = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 up_vector_ = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::vec3 euler_on_position_ = glm::vec3(0.0f);
  glm::mat4 view_ = glm::lookAt(position_, target_, up_vector_);
};

}  // render

#endif  // CORE_RENDER_CAMERA_H_
