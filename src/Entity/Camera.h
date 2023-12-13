#pragma once
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <_Commons/vertex.h>
#include <xnamath.h>
#include <unordered_map>

class Camera {
 public:
  Camera(float FOV, const position& xyz);
  void move(const float dx, const float dy, const float dz);
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
  XMMATRIX m_world_;

  position pos;
  int restrict_x;
  int restrict_y;
  int mx, my;
  
  float radians_x;
  float radians_y;
  float fov;
};
