#pragma once
#include <Rendering/Device.h>
#include <xnamath.h>
struct AdvVertex {
  XMFLOAT3 pos;
  XMFLOAT4 color;
};

class Pyramid {
 public:
  Pyramid(ID3D11VertexShader* v, ID3D11PixelShader* p, D3D_PRIMITIVE_TOPOLOGY drawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  void render();

  ID3D11Buffer* constBuffer() {
    return constBuffer_;
  }

 protected:
  ID3D11Buffer* vertexBuffer;
  ID3D11Buffer* indexBuffer;
  ID3D11Buffer* constBuffer_;
  ID3D11VertexShader* vShader;
  ID3D11PixelShader* pShader;

  D3D11_SUBRESOURCE_DATA setResData(void* vert);
  D3D11_BUFFER_DESC setBufferDesc(int byteWidth, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
};
