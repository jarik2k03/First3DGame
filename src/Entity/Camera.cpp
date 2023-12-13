#include "Camera.h"
#include <iomanip>
#include <algorithm>

Camera::Camera(float FOV, const position& xyz) : fov(FOV), mouse_x(0), mouse_y(0), pos(xyz) {
  move({pos.x, pos.y, pos.z});
  restrict_x = static_cast<int>(static_cast<float>(Window::height_) * XM_2PI);
  restrict_y = static_cast<int>(static_cast<float>(Window::height_) * 1.5 * XM_PIDIV4);

  m_proj_ = XMMatrixPerspectiveFovLH(FOV, Window::width_ / (float)Window::height_, 0.01f, 100.0f);
  // 3 аргумент - самое ближнее видимое расстояние, 4 аргумент - самое дальнее
}

void Camera::rotate(int dx, int dy) {
  mouse_x -= dx, mouse_y -= dy;  
  mouse_y = std::clamp(mouse_y, -restrict_y, restrict_y);
  mouse_x %= restrict_x;

  float radians_x = (float)mouse_x / Window::height_;
  float radians_y = (float)mouse_y / Window::height_;

  auto r = XMMatrixRotationY(radians_x) * XMMatrixRotationX(radians_y);
  m_view_ = XMMatrixTranslation(pos.x, pos.y, pos.z) * r;
}

void Camera::move_side(const float speed) {
  float radians = (float)mouse_x / Window::height_;
  float rot_x = sinf(radians);
  float rot_z = cosf(radians);

  const float calc_dx = speed * rot_x, calc_dz = speed * rot_z;
  pos.z -= calc_dx; // calc_dx;
  pos.x -= calc_dz; // calc_dz;
  m_view_ *= XMMatrixTranslation(-calc_dz, 0, -calc_dx);
}

void Camera::move_straight(const float speed) {
  float radians = (float)mouse_x / Window::height_;
  float rot_x = sinf(radians);
  float rot_z = cosf(radians);

  const float calc_dx = speed * rot_x, calc_dz = speed * rot_z;
  pos.z -= calc_dz; // calc_dx;
  pos.x += calc_dx; // calc_dz;
  m_view_ *= XMMatrixTranslation(calc_dx, 0, -calc_dz);
}

void Camera::move(const position& offset) {
  pos.x -= offset.x, pos.y -= offset.y, pos.z -= offset.z;
  m_view_ *= XMMatrixTranslation(offset.x, offset.y, offset.z);
}