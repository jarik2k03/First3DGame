#include "Block.hh"

Block::Block() {
}

Block::Block(uint16_t id, uint8_t x, uint8_t y, uint8_t z) : id(id), x(x), y(y), z(z) {
}

Block::~Block() {
}

void Block::set_pos(uint8_t x, uint8_t y, uint8_t z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

void Block::render(const XMFLOAT3& chunk_pos, ID3D11Buffer** const_buf_) {
  m_world = XMMatrixTranslation(chunk_pos.x + x, chunk_pos.y + y, chunk_pos.z + z);
  
  m_world = XMMatrixTranspose(m_world);
  
  Device::ic->UpdateSubresource(*const_buf_, 0, NULL, &m_world, 0, 0);
  Device::ic->DrawIndexed(36, 0, 0);
}