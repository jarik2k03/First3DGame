#include "Cube.h"

#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Cube::Cube(const position& xyz, ID3D11VertexShader* v, ID3D11PixelShader* p, D3D_PRIMITIVE_TOPOLOGY drawMode)
    : vShader(v), pShader(p), pos(xyz) {
  auto vertices = init_position(pos.x, pos.y, pos.z);
  WORD indices[36] = {
      1, 0, 2, 
      0, 1, 4, 
      2, 0, 4, 
      1, 2, 3,  
      1, 3, 5, 
      4, 1, 5, 
      5, 3, 7, 
      6, 5, 7, 
      2, 4, 6, 
      4, 5, 6, 
      2, 6, 7, 
      3, 2, 7};
  
  D3D11_BUFFER_DESC bd = setBufferDesc(sizeof(AdvVertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER);
  D3D11_SUBRESOURCE_DATA srd = setResData(vertices.data());
  HRESULT hr = Device::d3d->CreateBuffer(&bd, &srd, &vertexBuffer);
  H_WARNMSG(hr, L"Ошибка инициализации вершинного буфера");

  bd = setBufferDesc(sizeof(WORD) * 36, D3D11_BIND_INDEX_BUFFER);
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

verts Cube::init_position(const float x, const float y, const float z) {
  verts cubick(8);
  cubick.at(0) = {XMFLOAT3(x, y, z), XMFLOAT4(0.5f, 0.5f, 0.5f, 0.5f)};
  cubick.at(1) = {XMFLOAT3(x + 2, y, z), XMFLOAT4(0.6f, 0.6f, 0.6f, 0.6f)},
  cubick.at(2) = {XMFLOAT3(x, y, z + 2), XMFLOAT4(0.7f, 0.7f, 0.7f, 1.0f)},
  cubick.at(3) = {XMFLOAT3(x + 2, y, z + 2), XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f)},
  cubick.at(4) = {XMFLOAT3(x, y - 2, z), XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f)};
  cubick.at(5) = {XMFLOAT3(x + 2, y - 2, z), XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f)};
  cubick.at(6) = {XMFLOAT3(x, y - 2, z + 2), XMFLOAT4(0.3f, 0.3f, 0.3f, 1.0f)};
  cubick.at(7) = {XMFLOAT3(x + 2, y - 2, z + 2), XMFLOAT4(0.4f, 0.4f, 0.4f, 0.4f)};
  location_ = XMMatrixTranslation(x, y, z);
  return cubick;
}

void Cube::update_state(XMMATRIX& view, XMMATRIX& proj) {
  ConstantBuffer cb;
  cb.world = XMMatrixTranspose(location_);
  cb.view = XMMatrixTranspose(view);
  cb.proj = XMMatrixTranspose(proj);
  Device::ic->UpdateSubresource(constBuffer_, 0, NULL, &cb, 0, 0);

}

void Cube::render() {
  Device::ic->VSSetShader(vShader, NULL, 0);
  Device::ic->PSSetShader(pShader, NULL, 0);
  Device::ic->VSSetConstantBuffers(0, 1, &constBuffer_); // загрузка буфера в шейдер
  Device::ic->PSSetConstantBuffers(0, 1, &constBuffer_); // загрузка буфера в шейдер
  Device::ic->DrawIndexed(36, 0, 0);
}

D3D11_BUFFER_DESC Cube::setBufferDesc(int byteWidth, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = usage;
  bd.ByteWidth = byteWidth;
  bd.BindFlags = bindBuffer; // - тип буфера буфер вершин
  bd.CPUAccessFlags = 0;
  return bd;
}

D3D11_SUBRESOURCE_DATA Cube::setResData(void* vert) {
  D3D11_SUBRESOURCE_DATA bdata;
  ZeroMemory(&bdata, sizeof(bdata));
  bdata.pSysMem = vert;
  return bdata;
}