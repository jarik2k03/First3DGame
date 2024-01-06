#include "Camera.hh"
#include <algorithm>
#include <iomanip>

Camera::Camera(float FOV_angle, const XMFLOAT3& xyz) : fov(FOV_angle * XM_PI / 180), mouse_x(0), mouse_y(0), pos(xyz) {
  restrict_x = static_cast<int>(static_cast<float>(Window::height_) * XM_2PI);
  restrict_y = static_cast<int>(static_cast<float>(Window::height_) * 2 * XM_PIDIV4);

  XMMATRIX m_proj_ = XMMatrixPerspectiveFovLH(fov, Window::width_ / (float)Window::height_, 0.01f, 100.0f);
  m_proj_ = XMMatrixTranspose(m_proj_);
  const auto bd = set_const_buf(sizeof(ProjBuffer), D3D11_USAGE_DEFAULT);
  const auto hr_v = Device::d3d->CreateBuffer(&bd, NULL, &view_buf_);
  D3D11_SUBRESOURCE_DATA init = {0};
  init.pSysMem = &m_proj_;
  const auto hr_p = Device::d3d->CreateBuffer(&bd, &init, &proj_buf_);
  Device::ic->VSSetConstantBuffers(3, 1, &proj_buf_);
}




void Camera::rotate(int dx, int dy) {
  mouse_x -= dx, mouse_y -= dy;
  mouse_y = std::clamp(mouse_y, -restrict_y, restrict_y);
  mouse_x %= restrict_x;
}

void Camera::move_side(const float speed) {
  float radians = (float)mouse_x / Window::height_;
  float rot_x = sinf(radians);
  float rot_z = cosf(radians);

  const float calc_dx = speed * rot_x, calc_dz = speed * rot_z;
  pos.z -= calc_dx; // calc_dx;
  pos.x -= calc_dz; // calc_dz;
}

void Camera::move_straight(const float speed) {
  float radians = (float)mouse_x / Window::height_;
  float rot_x = sinf(radians);
  float rot_z = cosf(radians);

  const float calc_dx = speed * rot_x, calc_dz = speed * rot_z;
  pos.z -= calc_dz; // calc_dx;
  pos.x += calc_dx; // calc_dz;
}

void Camera::move(const XMFLOAT3& offset) {
  pos.x -= offset.x, pos.y -= offset.y, pos.z -= offset.z;
}

void Camera::update() {
  float radians_x = (float)mouse_x / Window::height_;
  float radians_y = (float)mouse_y / Window::height_;
  auto r = XMMatrixRotationY(radians_x) * XMMatrixRotationX(radians_y);
  XMMATRIX m_view_ = XMMatrixTranslation(pos.x, pos.y, pos.z) * r;
  m_view_ = XMMatrixTranspose(m_view_);
  Device::ic->UpdateSubresource(view_buf_, 0, NULL, &m_view_, 0, 0);
  Device::ic->VSSetConstantBuffers(2, 1, &view_buf_);

  //sstream ss;
  //ss << "X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z << '\n';
  //COUT(ss);
}



D3D11_BUFFER_DESC Camera::set_const_buf(int byte_width, D3D11_USAGE usage) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = usage;
  bd.ByteWidth = byte_width;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // - тип буфера буфер вершин
  bd.CPUAccessFlags = 0;
  return bd;
}