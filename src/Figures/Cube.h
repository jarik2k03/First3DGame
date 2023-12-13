#pragma once
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <_Commons/vertex.h>
#include <xnamath.h>
#include <vector>

using verts = std::vector<AdvVertex>;
using indices = std::vector<DWORD>;

class Cube {
 public:
  Cube(
      const position& xyz,
      ID3D11VertexShader* v,
      ID3D11PixelShader* p,
      D3D_PRIMITIVE_TOPOLOGY drawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  void update_state(XMMATRIX& view, XMMATRIX& proj);
  void render();
  verts init_position(const float x, const float y, const float z);
  indices init_indexed();
  ID3D11Buffer* constBuffer() {
    return constBuffer_;
  }

 protected:
  position pos;
  ID3D11Buffer* vertexBuffer;
  ID3D11Buffer* indexBuffer;
  ID3D11Buffer* constBuffer_;
  ID3D11VertexShader* vShader;
  ID3D11PixelShader* pShader;
  ID3D11SamplerState* sampler_buffer;
  ID3D11ShaderResourceView* textureSRV;
  XMMATRIX location_;

  D3D11_SUBRESOURCE_DATA set_res_data(void* vert);
  D3D11_BUFFER_DESC set_buffer_desc(int byteWidth, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
  D3D11_SAMPLER_DESC set_sampler_desc(
      D3D11_COMPARISON_FUNC cmp = D3D11_COMPARISON_NEVER,
      D3D11_FILTER mip = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
      float minLOD = 0.0f,
      float maxLOD = D3D11_FLOAT32_MAX);
};
