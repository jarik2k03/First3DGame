#include "Pyramid.h"

#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Pyramid::Pyramid(ID3D11VertexShader* v, ID3D11PixelShader* p, D3D_PRIMITIVE_TOPOLOGY drawMode) : vShader(v), pShader(p) {
	AdvVertex vertices[] =
    {	/* координаты X, Y, Z				цвет R, G, B, A					 */
        { XMFLOAT3(  0.0f,  1.5f,  0.0f ), XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  0.0f, -1.0f ), XMFLOAT4( 0.0f, 1.0f, 0.0f, 1.0f ) },
        { XMFLOAT3(  1.0f,  0.0f, -1.0f ), XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f ) },
        { XMFLOAT3( -1.0f,  0.0f,  1.0f ), XMFLOAT4( 0.0f, 1.0f, 1.0f, 1.0f ) },
        { XMFLOAT3(  1.0f,  0.0f,  1.0f ), XMFLOAT4( 1.0f, 0.0f, 1.0f, 1.0f ) }
    };

  WORD indices[18] = {0, 2, 1, 0, 3, 4, 0, 1, 3, 0, 4, 2, 1, 2, 3, 2, 4, 3};

  /* индексы массива vertices[], по которым строятся треугольники
   *  0,2,1,	 Треугольник 1 = vertices[0], vertices[2], vertices[3]
   *  0,3,4,	 Треугольник 2 = vertices[0], vertices[3], vertices[4]
   *  0,1,3,	 и т. д.
   *  0,4,2,
   *  1,2,3,
   *  2,4,3 */

  D3D11_BUFFER_DESC bd = setBufferDesc(sizeof(AdvVertex) * 5, D3D11_BIND_VERTEX_BUFFER);
  D3D11_SUBRESOURCE_DATA srd = setResData(vertices);
  HRESULT hr = Device::d3d->CreateBuffer(&bd, &srd, &vertexBuffer);
  H_WARNMSG(hr, L"Ошибка инициализации вершинного буфера");

  bd = setBufferDesc(sizeof(WORD) * 18, D3D11_BIND_INDEX_BUFFER);
  srd = setResData(indices);
  hr = Device::d3d->CreateBuffer(&bd, &srd, &indexBuffer);
  H_WARNMSG(hr, L"Ошибка инициализации индексного буфера");

  unsigned int stride = sizeof(AdvVertex);
  unsigned int offset = 0;

  Device::ic->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
  Device::ic->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
  Device::ic->IASetPrimitiveTopology(drawMode);

  bd = setBufferDesc(sizeof(XMMATRIX) * 3, D3D11_BIND_CONSTANT_BUFFER);
  hr = Device::d3d->CreateBuffer(&bd, NULL, &constBuffer_);
  H_WARNMSG(hr, L"Ошибка инициализации константного буфера");

  
}

void Pyramid::render() {
  Device::ic->VSSetShader(vShader, NULL, 0);
  Device::ic->PSSetShader(pShader, NULL, 0);
  Device::ic->VSSetConstantBuffers(0, 1, &constBuffer_); // загрузка буфера в шейдер
  Device::ic->DrawIndexed(18, 0, 0);
}

D3D11_BUFFER_DESC Pyramid::setBufferDesc(int byteWidth, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = usage;
  bd.ByteWidth = byteWidth;
  bd.BindFlags = bindBuffer; // - тип буфера буфер вершин
  bd.CPUAccessFlags = 0;
  return bd;
}

D3D11_SUBRESOURCE_DATA Pyramid::setResData(void* vert) {
  D3D11_SUBRESOURCE_DATA bdata;
  ZeroMemory(&bdata, sizeof(bdata));
  bdata.pSysMem = vert;
  return bdata;
}