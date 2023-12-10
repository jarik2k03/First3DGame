#pragma once
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <_Commons/vertex.h>
#include <xnamath.h>
#include <unordered_map>

using entity_buflist = std::unordered_map<stlstr, ID3D11Buffer*>;

class Camera {
 public:
  Camera(XMMATRIX& in_world, FLOAT objectiveWidth);
  void fix_position(XMMATRIX& in_world, ID3D11Buffer* buffer);
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
  XMMATRIX& m_world;
  float fov;
  float x, y, z;
  float rotation_anglex;
  float rotation_angley;
  float radians_x;
  float radians_y;
};
