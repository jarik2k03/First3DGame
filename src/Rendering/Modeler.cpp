#include "Modeler.hh"

#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);


void Modeler::create_model(stlcwstr& name) {
  model_buffer main_buffer;
  auto vertices = init_position();
  auto indices = init_indexed();
  
  main_buffer.idx_len = indices.size();

  auto bd = set_buffer_desc(sizeof(AdvVertex) * vertices.size(), D3D11_BIND_VERTEX_BUFFER);
  D3D11_SUBRESOURCE_DATA srd = set_res_data(vertices.data());
  HRESULT hr = Device::d3d->CreateBuffer(&bd, &srd, &main_buffer.vertex);
  H_WARNMSG(hr, L"Ошибка инициализации вершинного буфера");

  bd = set_buffer_desc(sizeof(WORD) * indices.size(), D3D11_BIND_INDEX_BUFFER);
  srd = set_res_data(indices.data());
  hr = Device::d3d->CreateBuffer(&bd, &srd, &main_buffer.index);
  H_WARNMSG(hr, L"Ошибка инициализации индексного буфера");

  hr = D3DX11CreateShaderResourceViewFromFileW(Device::d3d, L"assets/textures/dirt_.png", NULL, NULL, &main_buffer.texture, NULL);
  H_WARNMSG(hr, L"Не создалась текстура (возможно, файл не найден).");

  auto sample = set_sampler_desc(D3D11_COMPARISON_NEVER, D3D11_FILTER_MIN_MAG_MIP_LINEAR);
  hr = Device::d3d->CreateSamplerState(&sample, &main_buffer.sample);
  H_WARNMSG(hr, L"Создание образца текстуры выполнилось с ошибкой");
  saved_models.insert({name, main_buffer});
}

model_buffer Modeler::get_model(stlcwstr& name) const {
  auto it = saved_models.find(name);
  if (it == saved_models.end())
    std::runtime_error err("Текстура для загрузки не найдена!");
  return it->second;
}

void Modeler::remove_model(stlcwstr& name) {
  auto it = saved_models.find(name);
  if (it == saved_models.end())
    return;

  model_buffer erasing = it->second;
  erasing.vertex->Release();
  erasing.index->Release();
  erasing.sample->Release();
  erasing.texture->Release();
  saved_models.erase(it);
}



D3D11_BUFFER_DESC Modeler::set_buffer_desc(int byteWidth, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = usage;
  bd.ByteWidth = byteWidth;
  bd.BindFlags = bindBuffer; // - тип буфера буфер вершин
  bd.CPUAccessFlags = 0;
  return bd;
}

D3D11_SAMPLER_DESC Modeler::set_sampler_desc(D3D11_COMPARISON_FUNC cmp, D3D11_FILTER mip, float minLOD, float maxLOD) {
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

D3D11_SUBRESOURCE_DATA Modeler::set_res_data(void* vert) {
  D3D11_SUBRESOURCE_DATA bdata;
  ZeroMemory(&bdata, sizeof(bdata));
  bdata.pSysMem = vert;
  return bdata;
}

vertices init_position() {
  vertices cubick(24);
  float fix = 2.0f; // UP DIRECTION
  cubick.at(0) = {XMFLOAT3(-2, fix, -2), XMFLOAT2(X0, Z0)};
  cubick.at(1) = {XMFLOAT3(2, fix, -2), XMFLOAT2(X1, Z0)};
  cubick.at(2) = {XMFLOAT3(-2, fix, 2), XMFLOAT2(X0, Z1)};
  cubick.at(3) = {XMFLOAT3(2, fix, 2), XMFLOAT2(X1, Z1)};
  fix = 2.0f; // NORTH DIRECTION
  cubick.at(4) = {XMFLOAT3(fix, 2, -2), XMFLOAT2(X0, Z0)};
  cubick.at(5) = {XMFLOAT3(fix, 2, 2), XMFLOAT2(X0, Z1)};
  cubick.at(6) = {XMFLOAT3(fix, -2, -2), XMFLOAT2(X1, Z0)};
  cubick.at(7) = {XMFLOAT3(fix, -2, 2), XMFLOAT2(X1, Z1)};
  fix = -2.0f; // SOUTH DIRECTION
  cubick.at(8) = {XMFLOAT3(fix, 2, -2), XMFLOAT2(X0, Z0)};
  cubick.at(9) = {XMFLOAT3(fix, 2, 2), XMFLOAT2(X0, Z1)};
  cubick.at(10) = {XMFLOAT3(fix, -2, -2), XMFLOAT2(X1, Z0)};
  cubick.at(11) = {XMFLOAT3(fix, -2, 2), XMFLOAT2(X1, Z1)};
  fix = 2.0f; // EAST DIRECTION
  cubick.at(12) = {XMFLOAT3(2, 2, fix), XMFLOAT2(X0, Z0)};
  cubick.at(13) = {XMFLOAT3(-2, 2, fix), XMFLOAT2(X0, Z1)};
  cubick.at(14) = {XMFLOAT3(2, -2, fix), XMFLOAT2(X1, Z0)};
  cubick.at(15) = {XMFLOAT3(-2, -2, fix), XMFLOAT2(X1, Z1)};
  fix = -2.0f; // WEST DIRECTION
  cubick.at(16) = {XMFLOAT3(2, 2, fix), XMFLOAT2(X0, Z0)};
  cubick.at(17) = {XMFLOAT3(-2, 2, fix), XMFLOAT2(X0, Z1)};
  cubick.at(18) = {XMFLOAT3(2, -2, fix), XMFLOAT2(X1, Z0)};
  cubick.at(19) = {XMFLOAT3(-2, -2, fix), XMFLOAT2(X1, Z1)};
  fix = -2.0f; // DOWN DIRECTON
  cubick.at(20) = {XMFLOAT3(-2, fix, -2), XMFLOAT2(X0, Z0)};
  cubick.at(21) = {XMFLOAT3(2, fix, -2), XMFLOAT2(X0, Z1)};
  cubick.at(22) = {XMFLOAT3(-2, fix, 2), XMFLOAT2(X1, Z0)};
  cubick.at(23) = {XMFLOAT3(2, fix, 2), XMFLOAT2(X1, Z1)};

  return cubick;
}

indices init_indexed() {
  return indices(
      {1, 0, 3, 0, 2, 3, 4, 5, 7, 6, 4, 7, 9, 8, 11, 8, 10, 11, 12, 13, 15, 14, 12, 15, 17, 16, 19, 16, 18, 19, 21, 23, 20, 23, 22, 20});
}