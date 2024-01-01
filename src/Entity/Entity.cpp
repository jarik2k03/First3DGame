#include "Entity.hh"

#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Entity::Entity(const XMFLOAT3& xyz, const std::pair<stlwstr, model_buffer>& b, ID3D11VertexShader* v, ID3D11PixelShader* p)
    : vShader(v),
      pShader(p),
      n_indices(b.second.idx_len),
      pos{-xyz.x, -xyz.y, -xyz.z},
      vertex_buf_(b.second.vertex),
      index_buf_(b.second.index),
      textureSRV(b.second.texture),
      sampler_buf_(b.second.sample),
      model_name(b.first),
      const_light_buf_(NULL){

  const auto bd = set_const_buf(sizeof(PositionBuffer), D3D11_USAGE_DEFAULT);
  const auto hr = Device::d3d->CreateBuffer(&bd, NULL, &const_buf_);
  H_WARNMSG(hr, L"Ошибка инициализации константного буфера");

}

void Entity::set_glowing(bool truth) {
  const auto bd = set_const_buf(sizeof(LightBuffer), D3D11_USAGE_DEFAULT); 
  const auto hr = Device::d3d->CreateBuffer(&bd, NULL, &const_light_buf_);
  H_WARNMSG(hr, L"Ошибка инициализации светового буфера");
}

void Entity::move(const XMFLOAT3& offset) {
  pos.x += offset.x;
  pos.y += offset.y;
  pos.z += offset.z;
}

void Entity::update_state() {
  if (const_light_buf_) {
    LightBuffer lb;
    lb.dir = XMFLOAT4(pos.x, pos.y, pos.z, 0);
    lb.src_color = XMFLOAT4(1, 1, 1, 0);
    lb.dest_color = XMFLOAT4(1, 1, 1, 0);
    Device::ic->UpdateSubresource(const_light_buf_, 0, NULL, &lb, 0, 0);
  }
  m_world = XMMatrixTranslation(pos.x, pos.y, pos.z);  
  m_world = XMMatrixTranspose(m_world);
  Device::ic->UpdateSubresource(const_buf_, 0, NULL, &m_world, 0, 0);


}


D3D11_BUFFER_DESC Entity::set_const_buf(int byte_width, D3D11_USAGE usage) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = usage;
  bd.ByteWidth = byte_width;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // - тип буфера буфер вершин
  bd.CPUAccessFlags = 0;
  return bd;
}

void Entity::render() {
  unsigned int stride = sizeof(AdvVertex);
  unsigned int offset = 0;
  // биндим наши буферы к InputAssembler'у
  Device::ic->IASetVertexBuffers(0, 1, &vertex_buf_, &stride, &offset);
  Device::ic->IASetIndexBuffer(index_buf_, DXGI_FORMAT_R16_UINT, 0);
  Device::ic->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

  // биндим шейдеры
  Device::ic->VSSetShader(vShader, NULL, 0);
  Device::ic->PSSetShader(pShader, NULL, 0);
  // загрузка буфера в шейдер
  Device::ic->VSSetConstantBuffers(0, 1, &const_buf_);
  if (const_light_buf_) {
    Device::ic->VSSetConstantBuffers(1, 1, &const_light_buf_);
    Device::ic->PSSetConstantBuffers(1, 1, &const_light_buf_);  
  }
  // загрузка текстуры в шейдер
  Device::ic->PSSetShaderResources(0, 1, &textureSRV);
  Device::ic->PSSetSamplers(0, 1, &sampler_buf_);
  // готовый результат отобразится на экране
  Device::ic->DrawIndexed(n_indices, 0, 0);
}
