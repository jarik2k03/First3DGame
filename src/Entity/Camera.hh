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

 protected:
  D3D11_BUFFER_DESC set_const_buf(int byte_width, D3D11_USAGE usage);
  ID3D11Buffer* view_buf_;
  ID3D11Buffer* proj_buf_;
  XMFLOAT3 pos;
  int restrict_x, restrict_y;
  int mouse_x, mouse_y;
  float fov;
};

#endif // !CAMERA_HH