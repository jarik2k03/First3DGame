#pragma once
#ifndef ENTITY_HH
#define ENTITY_HH
#include <Entity/AEntity.hh>

class Entity : public AEntity {
 public:
  void update_state() override;
  Entity(const XMFLOAT3& xyz, const std::pair<stlwstr, model_buffer>& b, ID3D11VertexShader* v, ID3D11PixelShader* p);
  void set_glowing(bool truth);
  void render() override;
  void move(const XMFLOAT3& offset);

  ID3D11Buffer* const_buffer() {
    return const_buf_;
  }

 protected:
  XMFLOAT3 pos;
  WORD n_indices;
  stlwstr model_name;
  
  ID3D11Buffer* vertex_buf_;
  ID3D11Buffer* index_buf_;
  ID3D11Buffer* const_buf_;
  ID3D11Buffer* const_light_buf_;

  ID3D11SamplerState* sampler_buf_;
  ID3D11ShaderResourceView* textureSRV;

  ID3D11VertexShader* vShader;
  ID3D11PixelShader* pShader;
  D3D11_BUFFER_DESC set_const_buf(int byte_width, D3D11_USAGE usage) override;
};

#endif