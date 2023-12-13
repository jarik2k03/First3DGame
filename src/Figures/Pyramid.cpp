#include "Pyramid.h"

#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Pyramid::Pyramid(const float x, const float y, const float z, ID3D11VertexShader* v, ID3D11PixelShader* p, D3D_PRIMITIVE_TOPOLOGY drawMode)
    : vShader(v), pShader(p) {
  auto vertices = init_position(x, y, z);
  WORD indices[18] = {0, 2, 1, 0, 3, 4, 0, 1, 3, 0, 4, 2, 1, 2, 3, 2, 4, 3};

  D3D11_BUFFER_DESC bd = setBufferDesc(sizeof(AdvVertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER);
  D3D11_SUBRESOURCE_DATA srd = setResData(vertices.data());
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
  sstream ss;
  ss << "LIPTON CBUF: " << &constBuffer_ << "\n\n";
  CONSOLEDEBUG(ss);
}

verts Pyramid::init_position(const float x, const float y, const float z) {
  verts pyramidka(5);
  pyramidka.at(0) = {XMFLOAT3(x, y, z), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)};
  pyramidka.at(1) = {XMFLOAT3(x - 1, y - 1.5, z - 1), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
  pyramidka.at(2) = {XMFLOAT3(x + 1, y - 1.5, z - 1), XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)},
  pyramidka.at(3) = {XMFLOAT3(x - 1, y - 1.5, z + 1), XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)},
  pyramidka.at(4) = {XMFLOAT3(x + 1, y - 1.5, z + 1), XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)};
  location_ = XMMatrixTranslation(x, y, z);

  return pyramidka;
}

void Pyramid::update_state(XMMATRIX& view, XMMATRIX& proj) {
  ConstantBuffer cb;
  cb.world = XMMatrixTranspose(location_);
  cb.view = XMMatrixTranspose(view);
  cb.proj = XMMatrixTranspose(proj);
  Device::ic->UpdateSubresource(constBuffer_, 0, NULL, &cb, 0, 0);
}

void Pyramid::render() {
  Device::ic->VSSetShader(vShader, NULL, 0);
  Device::ic->PSSetShader(pShader, NULL, 0);
  Device::ic->VSSetConstantBuffers(0, 1, &constBuffer_); // загрузка буфера в шейдер
  Device::ic->PSSetConstantBuffers(0, 1, &constBuffer_); // загрузка буфера в шейдер
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