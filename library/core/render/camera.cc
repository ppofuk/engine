#include "core/render/camera.h"
#include <cassert>

namespace render {

void Camera::RecalculateViewMatrix() {
  view_ = glm::lookAt(position_, target_, up_vector_);
  view_has_updated_ = true;
}

void Camera::RecalculateViewMatrixWithRotation() {
  assert(false);
  view_ = glm::lookAt(position_, target_, up_vector_);
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

void Camera::ApplyRotationAroundPosition(const glm::vec3& rotation) {
  euler_on_position_ = rotation;
  glm::vec3 rotated = glm::eulerAngleYXZ(glm::radians(euler_on_position_.x),
                                         glm::radians(euler_on_position_.y),
                                         glm::radians(euler_on_position_.z)) *
                      glm::vec4(position_.x, position_.y, position_.z, 1.0f);
  position_ = rotated;
  RecalculateViewMatrix();
}

void Camera::Move(const glm::vec3& position) {
  view_ = glm::translate(position) * view_;
  view_has_updated_ = true;
}

void Camera::Rotate(const glm::vec3& rotation) {
  view_ = glm::eulerAngleYXZ(glm::radians(rotation.x), glm::radians(rotation.y),
                             glm::radians(rotation.z)) *
          view_;
  view_has_updated_ = true;
}

void Camera::RotateAround(const glm::vec3& rotation,
                          const glm::vec3& point_offset) {
  view_ = glm::translate(-point_offset) *
          glm::eulerAngleYXZ(glm::radians(rotation.x), glm::radians(rotation.y),
                             glm::radians(rotation.z)) *

          glm::translate(point_offset) * view_;

  view_has_updated_ = true;
}

bool Camera::HasViewUpdated() {
  if (view_has_updated_) {
    view_has_updated_ = false;
    return true;
  }
  return false;
}


}  // render
