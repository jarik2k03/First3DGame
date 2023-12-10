#include "Camera.h"

Camera::Camera(XMMATRIX& in_world, FLOAT objectiveWidth)
    : fov(objectiveWidth), rotation_anglex(0.0f), rotation_angley(0.0f), m_world(in_world) {
  x = 0.0f, y = 1.0f, z = -55.0f;
  FXMVECTOR eye = XMVectorSet(x, y, z, 0);
  FXMVECTOR rotation = XMVectorSet(0, 1, 0, 0);
  FXMVECTOR pitch = XMVectorSet(0, 1, 0, 0);

  m_view_ = XMMatrixLookAtLH(eye, rotation, pitch); // LH наша координатная система
  m_proj_ = XMMatrixPerspectiveFovLH(objectiveWidth, Window::width_ / (float)Window::height_, 0.01f, 100.0f);
  // 3 аргумент - самое ближнее видимое расстояние, 4 аргумент - самое дальнее
}

void Camera::fix_position(XMMATRIX& in_world, ID3D11Buffer* buffer) {
  ConstantBuffer bd;
  bd.world = XMMatrixTranspose(in_world);
  bd.view = XMMatrixTranspose(m_view_);
  bd.proj = XMMatrixTranspose(m_proj_);

  debug_matrixes(in_world, m_view_, m_proj_);
  // загружаем структуру bd в константный буфер
  Device::ic->UpdateSubresource(buffer, 0, NULL, &bd, 0, 0);
}

void Camera::rotate(int dx, int dy) {

  float radians_dx = (float)dx / Window::height_;
  float radians_dy = (float)dy / Window::height_;
  radians_x += radians_dx;
  radians_y -= radians_dy;

  XMVECTOR eye = XMVectorSet(x, y, z, 0);
  XMVECTOR rotation = XMVectorSet(radians_x, radians_y, 0, 1);
  XMVECTOR pitch = XMVectorSet(0, 1, 0, 0);

  m_view_ = XMMatrixLookAtLH(eye, rotation, pitch); // LH наша координатная система
}

void Camera::move(const float dx, const float dy, const float dz) {
  x += dx, y += dy, z += dz;
  FXMVECTOR eye = XMVectorSet(0, 0, 0, 1);
  FXMVECTOR rotation = XMVectorSet(0, 0, 0, 1);
  FXMVECTOR pitch = XMVectorSet(0, 1, 0, 1);
  m_view_ = XMMatrixLookAtLH(eye, rotation, pitch); // LH наша координатная система
  m_view_(3, 0) = x;
  m_view_(3, 1) = y;
  m_view_(3, 2) = z;
}