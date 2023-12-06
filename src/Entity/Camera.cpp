#include "Camera.h"

Camera::Camera(int w, int h, FLOAT objectiveWidth) : fov(objectiveWidth), rotation_angle(0.0f) {
  x = 0.0f, y = 0.0f, z = 5.0f;
  m_view_ = XMMatrixTranslation(x, y, z);
  //m_view_ = XMMatrixLookAtLH(eyePos, eyeDir, upDir); // LH наша координатная система
  m_proj_ = XMMatrixPerspectiveFovLH(objectiveWidth, w / (float)h, 0.01f, 100.0f);

  // 3 аргумент - самое ближнее видимое расстояние, 4 аргумент - самое дальнее
}

void Camera::fix_position(XMMATRIX& in_world, entity_buflist& figures) {
  ConstantBuffer bd;
  bd.world = XMMatrixTranspose(in_world);
  bd.view = XMMatrixTranspose(m_view_);
  bd.proj = XMMatrixTranspose(m_proj_);

  debug_matrixes(in_world, m_view_, m_proj_);
  // загружаем структуру bd в константный буфер
  for (const auto& it : figures) {
    Device::ic->UpdateSubresource(it.second, 0, NULL, &bd, 0, 0);
  }
}

void Camera::rotate(float angle) {
  rotation_angle += angle;
  

  // 00 02 20 22

}

void Camera::move(const float dx, const float dy, const float dz) {
  x += dx, y += dy, z += dz;
  XMMATRIX coord = XMMatrixTranslation(x, y, z);
  m_view_(3, 0) = coord(3, 0);
  m_view_(3, 1) = coord(3, 1);
  m_view_(3, 2) = coord(3, 2);
}