#include "World.h"

World::World() {
  m_world_ = XMMatrixIdentity();
}

XMMATRIX& World::m_world() {
  return m_world_;
}