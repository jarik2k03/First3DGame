#pragma once
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <_Commons/vertex.h>
#include <xnamath.h>
#include <unordered_map>

class Camera {
 public:
  Camera(float FOV, const position& xyz);
  void move(const position& offset);
  void move_side(float speed);
  void move_straight(float speed);
  void rotate(int dx, int dy);
  XMMATRIX& m_view() {
    return m_view_;
  }
  XMMATRIX& m_proj() {
    return m_proj_;
  }

 protected:
  XMMATRIX m_view_;
  XMMATRIX m_proj_;
  position pos;
  int restrict_x, restrict_y;
  int mouse_x, mouse_y;
  float fov;
};
