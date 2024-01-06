#include "Block.hh"

Block::Block() : id_(0), shown_faces(1) {
}

Block::Block(uint16_t id) : id_(id), shown_faces(rand() % 2){
}

Block::~Block() {
}

bool Block::is_shown() {
  return shown_faces;
}
uint16_t Block::id() {
  return id_;
}

void Block::render(ID3D11VertexShader* v) {
  Device::ic->DrawIndexed(36, 0, 0);
}