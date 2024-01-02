#include "Block.hh"

Block::Block() : id(0), shown_faces(1) {
}

Block::Block(uint16_t id) : id(id), shown_faces(1){
}

Block::~Block() {
}

bool Block::is_shown() {
  return shown_faces;
}

void Block::render(const XMFLOAT3& chunk_pos, ID3D11Buffer** const_buf_) {
  Device::ic->DrawIndexed(36, 0, 0);
}