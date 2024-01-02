#pragma once
#ifndef BLOCK_HH
#define BLOCK_HH
#include <Entity/AEntity.hh>

class Block {
 public:
  Block();
  Block(uint16_t id);
  ~Block();
  void render(const XMFLOAT3& chunk_pos, ID3D11Buffer** const_buf_);
  bool is_shown();
  uint16_t id();
 private:
  uint16_t id_;
  uint8_t shown_faces;
};

#endif