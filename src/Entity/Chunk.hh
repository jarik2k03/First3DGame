#pragma once
#ifndef CHUNK_HH
#define CHUNK_HH
#include <Entity/Block.hh>
#include <array>
#include <functional>

class Chunk {
 public:
  Chunk(
      int length,
      const XMFLOAT3& xyz,
      const std::pair<stlwstr, model_buffer>& b,
      ID3D11VertexShader* v,
      ID3D11PixelShader* p,
      ID3D11InputLayout* l);
  ~Chunk();
  void update_render();

 private:
  std::unique_ptr<Block, std::function<void(Block*)>> chunk;
  std::vector<uint32_t> shown_blocks;
  stlwstr model_name;
  XMFLOAT3 pos;
  WORD n_indices;
  uint8_t length;

  ID3D11Buffer* vertex_buf_;
  ID3D11Buffer* index_buf_;
  ID3D11Buffer* chunk_buf_;
  ID3D11Buffer* counter_buf_;
  ID3D11InputLayout* layout_;

  ID3D11SamplerState* sampler_buf_;
  ID3D11ShaderResourceView* textureSRV;

  ID3D11VertexShader* v_shader;
  ID3D11PixelShader* p_shader;

  D3D11_BUFFER_DESC set_const_buf(int byte_width, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access = (D3D11_CPU_ACCESS_FLAG)0);
};

#endif
