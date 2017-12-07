#include "core/render/camera.h"

namespace render {

void Camera::RecalculateViewMatrix() {
  view_ = glm::lookAt(position_, target_, up_vector_);
  view_has_updated_ = true;
}

void Camera::RecalculateViewMatrixWithRotation() {
  glm::vec3 camera_position_rotated =
      glm::eulerAngleYXZ(glm::radians(euler_on_position_.x),
                         glm::radians(euler_on_position_.y),
                         glm::radians(euler_on_position_.z)) *
      glm::vec4(position_.x, position_.y, position_.z, 1.0f);

  view_ = glm::lookAt(camera_position_rotated, target_, up_vector_);
  view_has_updated_ = true;
}

void Camera::LookAt(const glm::vec3& position,
                    const glm::vec3& target,
                    const glm::vec3& up_vector) {
  position_ = position;
  target_ = target;
  up_vector_ = up_vector;
  RecalculateViewMatrix();
}

void Camera::AddRotationAroundPosition(const glm::vec3& rotation) {
  euler_on_position_ += rotation;
  RecalculateViewMatrixWithRotation();
}

bool Camera::HasViewUpdated() {
  if (view_has_updated_) {
    view_has_updated_ = false;
    return true;
  }
  return false;
}


}  // render
