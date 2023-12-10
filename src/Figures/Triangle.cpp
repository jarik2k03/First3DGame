#include "Triangle.h"

#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Triangle::Triangle(
    ID3D11VertexShader* v,
    ID3D11PixelShader* p,
    TriangleVertices& i_firstCoords,
    D3D_PRIMITIVE_TOPOLOGY drawMode) {
  coordinates = i_firstCoords;
  vShader = NULL;
  pShader = NULL;

  D3D11_BUFFER_DESC bd;
  D3D11_SUBRESOURCE_DATA bufData; // структура содержит данные буфера
  setBuffer(&bd, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER);
  setBufferData(&bufData);

  HRESULT hr = Device::d3d->CreateBuffer(&bd, &bufData, &vertexBuffer);
  H_WARNMSG(hr, L"Ошибка инициализации буфера");
  unsigned int stride = sizeof(XMFLOAT3);
  unsigned int offset = 0;
  Device::ic->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
  Device::ic->IASetPrimitiveTopology(drawMode);
}

void Triangle::render(ID3D11DeviceContext* i_context) {
  i_context->VSSetShader(vShader, NULL, 0);
  i_context->PSSetShader(pShader, NULL, 0);
  i_context->Draw(3, 0);
}

void Triangle::setBuffer(D3D11_BUFFER_DESC* o_bd, D3D11_USAGE usage, D3D11_BIND_FLAG bindBuffer) {
  ZeroMemory(o_bd, sizeof(*o_bd));
  o_bd->Usage = usage;
  o_bd->ByteWidth = sizeof(XMFLOAT3) * 3;
  o_bd->BindFlags = D3D11_BIND_VERTEX_BUFFER; // - тип буфера буфер вершин
  o_bd->CPUAccessFlags = 0;
}

void Triangle::setBufferData(D3D11_SUBRESOURCE_DATA* o_bdata) {
  ZeroMemory(o_bdata, sizeof(*o_bdata));
  o_bdata->pSysMem = &coordinates;
}