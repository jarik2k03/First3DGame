#pragma once
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <_Commons/vertex.h>
#include <Rendering/Modeler.hh>
#include <xnamath.h>

class Cube {
 public:
  Cube(const XMFLOAT3& xyz, const model_buffer& b, ID3D11VertexShader* v, ID3D11PixelShader* p);
  void update_state(XMMATRIX& view, XMMATRIX& proj);
  void render();

  ID3D11Buffer* const_buffer() {
    return const_buf_;
  }

 protected:
  XMFLOAT3 pos;
  ID3D11Buffer* vertex_buf_;
  ID3D11Buffer* index_buf_;
  ID3D11Buffer* const_buf_;

  ID3D11SamplerState* sampler_buf_;
  ID3D11ShaderResourceView* textureSRV;

  ID3D11VertexShader* vShader;
  ID3D11PixelShader* pShader;

  D3D11_BUFFER_DESC set_const_buf(int byte_width, D3D11_USAGE usage);
};
