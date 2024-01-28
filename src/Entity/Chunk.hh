#pragma once
#ifndef CHUNK_HH
#define CHUNK_HH
#include <Controllers/Shaders.hh>
#include <Entity/Block.hh>
#include <array>
#include <functional>

class Chunk {
 public:
  Chunk(
      int length,
      const XMFLOAT3& xyz,
      const std::pair<stlwstr, model_buffer>& b,
      const shader_blob& compiled,
      ID3D11VertexShader* v,
      ID3D11PixelShader* p,
      ID3D11InputLayout* l);
  ~Chunk();
  void update_render();

 private:
  std::unique_ptr<Block, std::function<void(Block*)>> chunk;
  std::vector<uint32_t> shown_blocks;
  std::pair<stlwstr, model_buffer> model;
  XMFLOAT3 pos;
  stlstr temp;
  uint8_t length;

  ID3D11InputLayout* layout_;
  ID3D11VertexShader* v_shader;
  ID3D11PixelShader* p_shader;
};

#endif
