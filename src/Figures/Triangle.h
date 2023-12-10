#pragma once
//#include "AbstractFigure.h"
#include <Rendering/Device.h>
#include <xnamath.h>

struct TriangleVertices {
  XMFLOAT3 st, nd, rd;
};

class Triangle {
 public:
  Triangle(
      ID3D11VertexShader* v,
      ID3D11PixelShader* p,
      TriangleVertices& i_firstCoords,
      D3D_PRIMITIVE_TOPOLOGY drawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  void render(ID3D11DeviceContext* i_context);

 protected:
  ID3D11Buffer* vertexBuffer;
  ID3D11VertexShader* vShader;
  ID3D11PixelShader* pShader;
  TriangleVertices coordinates;

  void setBufferData(D3D11_SUBRESOURCE_DATA* bdata);
  void setBuffer(D3D11_BUFFER_DESC* bd, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, D3D11_BIND_FLAG bindBuffer = D3D11_BIND_VERTEX_BUFFER);
};
