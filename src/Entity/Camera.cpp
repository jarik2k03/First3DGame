#include "Camera.hh"
#include <algorithm>
#include <iomanip>

Camera::Camera(float FOV_angle, const XMFLOAT3& xyz) : fov(FOV_angle * XM_PI / 180), mouse_x(0), mouse_y(0), pos(xyz) {
  move({pos.x, pos.y, pos.z});
  restrict_x = static_cast<int>(static_cast<float>(Window::height_) * XM_2PI);
  restrict_y = static_cast<int>(static_cast<float>(Window::height_) * 2 * XM_PIDIV4);

  m_proj_ = XMMatrixPerspectiveFovLH(fov, Window::width_ / (float)Window::height_, 0.01f, 100.0f);
  const auto bd = set_const_buf(sizeof(CamBuffer), D3D11_USAGE_DEFAULT);
  const auto hr = Device::d3d->CreateBuffer(&bd, NULL, &cam_buf_);
  // 3 аргумент - самое ближнее видимое рассто€ние, 4 аргумент - самое дальнее
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

  sstream ss;
  ss << "X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z << '\n';
  CONSOLEDEBUG(ss);
}

void Camera::move_straight(const float speed) {
  float radians = (float)mouse_x / Window::height_;
  float rot_x = sinf(radians);
  float rot_z = cosf(radians);

  const float calc_dx = speed * rot_x, calc_dz = speed * rot_z;
  pos.z -= calc_dz; // calc_dx;
  pos.x += calc_dx; // calc_dz;
  m_view_ *= XMMatrixTranslation(calc_dx, 0, -calc_dz);

  sstream ss;
  ss << "X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z << '\n';
  CONSOLEDEBUG(ss);
}

void Camera::move(const XMFLOAT3& offset) {
  pos.x -= offset.x, pos.y -= offset.y, pos.z -= offset.z;
  m_view_ *= XMMatrixTranslation(offset.x, offset.y, offset.z);

  sstream ss;
  ss << "X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z << '\n';
  CONSOLEDEBUG(ss);
}

void Camera::update() {
  CamBuffer cb;
  cb.view = XMMatrixTranspose(m_view_);
  cb.proj = XMMatrixTranspose(m_proj_);
  Device::ic->UpdateSubresource(cam_buf_, 0, NULL, &cb, 0, 0);
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