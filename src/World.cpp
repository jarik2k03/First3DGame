#include "World.h"

World::World() {
  m_world_ = XMMatrixIdentity();
}

void World::UpdateMatrixes(XMMATRIX& view, XMMATRIX& proj, ID3D11Buffer* cBuffer) {
  static double time = 0.0f;
  static DWORD starttime = 0;
  DWORD curtime = GetTickCount64();
  if (starttime == 0)
    starttime = curtime;
  time = (curtime - starttime) / 1000.f;

  m_world_ = XMMatrixRotationY(time);

  ConstantBuffer bd;
  bd.world = XMMatrixTranspose(m_world_);
  bd.view = XMMatrixTranspose(view);
  bd.proj = XMMatrixTranspose(proj);

  // загружаем структуру bd в константный буфер
  Device::ic->UpdateSubresource(cBuffer, 0, NULL, &bd, 0, 0);
  // загружаем в константный буфер
}

void World::debug_matrixes(XMMATRIX& view, XMMATRIX& proj) {
  sstream ss;
  ss << "WORLD:\n";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ss << m_world_(i, j) << " ";
    }
    ss << "\n";
  }
  ss << "VIEW\n";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ss << view(i, j) << " ";
    }
    ss << "\n";
  }
  ss << "PROJECTION\n";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ss << proj(i, j) << " ";
    }
    ss << "\n";
  }
  ss << "\n";

  CONSOLEDEBUG(ss);
}

XMMATRIX& World::m_world() {
  return m_world_;
}