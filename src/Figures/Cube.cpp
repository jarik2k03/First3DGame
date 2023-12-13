#include "Cube.h"

#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Cube::Cube(const position& xyz, ID3D11VertexShader* v, ID3D11PixelShader* p, D3D_PRIMITIVE_TOPOLOGY drawMode)
    : vShader(v), pShader(p), pos(xyz) {
  auto vertices = init_position(pos.x, pos.y, pos.z);
  auto indices = init_indexed();

  D3D11_BUFFER_DESC bd = set_buffer_desc(sizeof(AdvVertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER);
  D3D11_SUBRESOURCE_DATA srd = set_res_data(vertices.data());
  HRESULT hr = Device::d3d->CreateBuffer(&bd, &srd, &vertexBuffer);
  H_WARNMSG(hr, L"Ошибка инициализации вершинного буфера");

  bd = set_buffer_desc(sizeof(WORD) * indices.size(), D3D11_BIND_INDEX_BUFFER);
  srd = set_res_data(indices.data());
  hr = Device::d3d->CreateBuffer(&bd, &srd, &indexBuffer);
  H_WARNMSG(hr, L"Ошибка инициализации индексного буфера");

  unsigned int stride = sizeof(AdvVertex);
  unsigned int offset = 0;
  Device::ic->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
  Device::ic->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R16_UINT, 0);
  Device::ic->IASetPrimitiveTopology(drawMode);

  bd = set_buffer_desc(sizeof(XMMATRIX) * 3, D3D11_BIND_CONSTANT_BUFFER);
  hr = Device::d3d->CreateBuffer(&bd, NULL, &constBuffer_);
  H_WARNMSG(hr, L"Ошибка инициализации константного буфера");

  hr = D3DX11CreateShaderResourceViewFromFileW(Device::d3d, L"assets/textures/dirt_.png", NULL, NULL, &textureSRV, NULL);
  H_WARNMSG(hr, L"Не создалась текстура (возможно, файл не найден).");

  auto sample = set_sampler_desc(D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
  hr = Device::d3d->CreateSamplerState(&sample, &sampler_buffer);
  H_WARNMSG(hr, L"Создание образца текстуры выполнилось с ошибкой");
}

verts Cube::init_position(const float x, const float y, const float z) {
  verts cubick(24);
  cubick.at(0) = {XMFLOAT3(x - 1.0f, y + 1.0f, z - 1.0f), XMFLOAT2(0.0f, 0.0f)};
  cubick.at(1) = {XMFLOAT3(x + 1.0f, y + 1.0f, z - 1.0f), XMFLOAT2(1.0f, 0.0f)};
  cubick.at(2) = {XMFLOAT3(x + 1.0f, y + 1.0f, z + 1.0f), XMFLOAT2(1.0f, 1.0f)};
  cubick.at(3) = {XMFLOAT3(x - 1.0f, y + 1.0f, z + 1.0f), XMFLOAT2(0.0f, 1.0f)};

  cubick.at(4) = {XMFLOAT3(x - 1.0f, y - 1.0f, z - 1.0f), XMFLOAT2(0.0f, 0.0f)};
  cubick.at(5) = {XMFLOAT3(x + 1.0f, y - 1.0f, z - 1.0f), XMFLOAT2(1.0f, 0.0f)};
  cubick.at(6) = {XMFLOAT3(x + 1.0f, y - 1.0f, z + 1.0f), XMFLOAT2(1.0f, 1.0f)};
  cubick.at(7) = {XMFLOAT3(x - 1.0f, y - 1.0f, z + 1.0f), XMFLOAT2(0.0f, 1.0f)};

  cubick.at(8) = {XMFLOAT3(x - 1.0f, y - 1.0f, z + 1.0f), XMFLOAT2(0.0f, 0.0f)};
  cubick.at(9) = {XMFLOAT3(x - 1.0f, y - 1.0f, z - 1.0f), XMFLOAT2(1.0f, 0.0f)};
  cubick.at(10) = {XMFLOAT3(x - 1.0f, y + 1.0f, z - 1.0f), XMFLOAT2(1.0f, 1.0f)};
  cubick.at(11) = {XMFLOAT3(x - 1.0f, y + 1.0f, z + 1.0f), XMFLOAT2(0.0f, 1.0f)};

  cubick.at(12) = {XMFLOAT3(x + 1.0f, y - 1.0f, z + 1.0f), XMFLOAT2(0.0f, 0.0f)};
  cubick.at(13) = {XMFLOAT3(x + 1.0f, y - 1.0f, z - 1.0f), XMFLOAT2(1.0f, 0.0f)};
  cubick.at(14) = {XMFLOAT3(x + 1.0f, y + 1.0f, z - 1.0f), XMFLOAT2(1.0f, 1.0f)};
  cubick.at(15) = {XMFLOAT3(x + 1.0f, y + 1.0f, z + 1.0f), XMFLOAT2(0.0f, 1.0f)};

  cubick.at(16) = {XMFLOAT3(x - 1.0f, y - 1.0f, z - 1.0f), XMFLOAT2(0.0f, 0.0f)};
  cubick.at(17) = {XMFLOAT3(x + 1.0f, y - 1.0f, z - 1.0f), XMFLOAT2(1.0f, 0.0f)};
  cubick.at(18) = {XMFLOAT3(x + 1.0f, y + 1.0f, z - 1.0f), XMFLOAT2(1.0f, 1.0f)};
  cubick.at(19) = {XMFLOAT3(x - 1.0f, y + 1.0f, z - 1.0f), XMFLOAT2(0.0f, 1.0f)};

  cubick.at(20) = {XMFLOAT3(x - 1.0f, y - 1.0f, z + 1.0f), XMFLOAT2(0.0f, 0.0f)};
  cubick.at(21) = {XMFLOAT3(x + 1.0f, y - 1.0f, z + 1.0f), XMFLOAT2(1.0f, 0.0f)};
  cubick.at(22) = {XMFLOAT3(x + 1.0f, y + 1.0f, z + 1.0f), XMFLOAT2(1.0f, 1.0f)};
  cubick.at(23) = {XMFLOAT3(x - 1.0f, y + 1.0f, z + 1.0f), XMFLOAT2(0.0f, 1.0f)};
  location_ = XMMatrixTranslation(x, y, z);
  return cubick;
}

indices Cube::init_indexed() {
  return indices(
      {3, 1, 0, 2, 1, 3, 6, 4, 5, 7, 4, 6, 11, 9, 8, 10, 9, 11, 14, 12, 13, 15, 12, 14, 19, 17, 16, 18, 17, 19, 22, 20, 21, 23, 20, 22});
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
  Device::ic->PSSetShaderResources(0, 1, &textureSRV);
  Device::ic->PSSetSamplers(0, 1, &sampler_buffer);
  Device::ic->DrawIndexed(36, 0, 0);
}

D3D11_BUFFER_DESC Cube::set_buffer_desc(int byteWidth, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = usage;
  bd.ByteWidth = byteWidth;
  bd.BindFlags = bindBuffer; // - тип буфера буфер вершин
  bd.CPUAccessFlags = 0;
  return bd;
}

D3D11_SAMPLER_DESC Cube::set_sampler_desc(D3D11_COMPARISON_FUNC cmp, D3D11_FILTER mip, float minLOD, float maxLOD) {
  D3D11_SAMPLER_DESC sd;
  ZeroMemory(&sd, sizeof(sd));
  sd.Filter = mip;
  sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  sd.ComparisonFunc = cmp;
  sd.MinLOD = minLOD;
  sd.MaxLOD = maxLOD;
  return sd;
}

D3D11_SUBRESOURCE_DATA Cube::set_res_data(void* vert) {
  D3D11_SUBRESOURCE_DATA bdata;
  ZeroMemory(&bdata, sizeof(bdata));
  bdata.pSysMem = vert;
  return bdata;
}