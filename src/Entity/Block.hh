#pragma once
#ifndef BLOCK_HH
#define BLOCK_HH
#include <Entity/AEntity.hh>

class Block {
 public:
  Block();
  Block(uint16_t id, uint8_t x, uint8_t y, uint8_t z);
  ~Block();
  void set_pos(uint8_t x, uint8_t y, uint8_t z);
  void render(const XMFLOAT3& chunk_pos, ID3D11Buffer** const_buf_);

 private:
  uint16_t id;
  uint8_t x, y, z;
};

#endif