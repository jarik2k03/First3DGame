#include "Camera.h"
#include <algorithm>

Camera::Camera(float FOV, const position& xyz)
    : fov(FOV), mx(0), my(0), pos(xyz){
  move(pos.x, pos.y ,pos.z);
  restrict_x = static_cast<int>(static_cast<float>(Window::height_) * XM_2PI);
  restrict_y = static_cast<int>(static_cast<float>(Window::height_) * 1.5 * XM_PIDIV4);

  m_proj_ = XMMatrixPerspectiveFovLH(FOV, Window::width_ / (float)Window::height_, 0.01f, 100.0f);
  // 3 аргумент - самое ближнее видимое расстояние, 4 аргумент - самое дальнее
}


void Camera::rotate(int dx, int dy) {
  mx -= dx, my -= dy;  
  my = std::clamp(my, -restrict_y, restrict_y);
  mx %= restrict_x;

  float radians_x = (float)mx / Window::height_;
  float radians_y = (float)my / Window::height_;
  
  auto r = XMMatrixRotationY(radians_x) * XMMatrixRotationX(radians_y);
  m_view_ = XMMatrixTranslation(pos.x, pos.y, pos.z) * r;
}


void Camera::move(const float dx, const float dy, const float dz) {
  pos.x += dx, pos.y += dy, pos.z += dz;
  XMMATRIX translated = XMMatrixTranslation(dx, dy, dz);
  m_view_ *= translated;
}