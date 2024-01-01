#pragma once
#ifndef CAMERA_HH
#define CAMERA_HH
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <_Commons/vertex.h>
#include <xnamath.h>

class Camera {
 public:
  Camera(float FOV, const XMFLOAT3& xyz);
  void move(const XMFLOAT3& offset);
  void move_side(const float speed);
  void move_straight(const float speed);
  void rotate(int dx, int dy);
  void update();
  XMMATRIX& m_view() {
    return m_view_;
  }
  XMMATRIX& m_proj() {
    return m_proj_;
  }

 protected:
  ID3D11Buffer* cam_buf_;
  XMMATRIX m_view_;
  XMMATRIX m_proj_;
  XMFLOAT3 pos;
  int restrict_x, restrict_y;
  int mouse_x, mouse_y;
  float fov;
};

#endif // !CAMERA_HH