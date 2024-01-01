#include "Chunk.hh"
#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Chunk::Chunk(int length, const XMFLOAT3& xyz, const std::pair<stlwstr, model_buffer>& b, ID3D11VertexShader* v, ID3D11PixelShader* p)
    : chunk(new Block[length * length * length], [](Block* chunk) { delete[] chunk; }),
      n_indices(b.second.idx_len),
      pos{-xyz.x, -xyz.y, -xyz.z},
      vertex_buf_(b.second.vertex),
      index_buf_(b.second.index),
      textureSRV(b.second.texture),
      sampler_buf_(b.second.sample),
      model_name(b.first),
      length(length),
      v_shader(v),
      p_shader(p) {
  auto* blocks = chunk.get();
  int volume = length * length * length;
  int area = length * length;
  for (size_t i = 0; i < volume; i++) {
    blocks[i].set_pos(i / length % length, i % length, i / area);
  }

  const auto bd = set_const_buf(sizeof(ConstantBuffer), D3D11_USAGE_DEFAULT);
  const auto hr = Device::d3d->CreateBuffer(&bd, NULL, &const_buf_);
  H_WARNMSG(hr, L"Ошибка инициализации константного буфера");
}

void Chunk::update_render(XMMATRIX& view, XMMATRIX& proj) {
  unsigned int stride = sizeof(AdvVertex);
  unsigned int offset = 0;
  Device::ic->IASetVertexBuffers(0, 1, &vertex_buf_, &stride, &offset);
  Device::ic->IASetIndexBuffer(index_buf_, DXGI_FORMAT_R16_UINT, 0);
  Device::ic->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  Device::ic->VSSetShader(v_shader, NULL, 0);
  Device::ic->PSSetShader(p_shader, NULL, 0);
  Device::ic->VSSetConstantBuffers(0, 1, &const_buf_);
  Device::ic->PSSetShaderResources(0, 1, &textureSRV);
  Device::ic->PSSetSamplers(0, 1, &sampler_buf_);

  int volume = length * length * length;
  auto* blocks = chunk.get();
  for (size_t i = 0; i < volume; i++) {
    blocks[i].render(pos, view, proj, &const_buf_);
  }
}

D3D11_BUFFER_DESC Chunk::set_const_buf(int byte_width, D3D11_USAGE usage) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = usage;
  bd.ByteWidth = byte_width;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // - тип буфера буфер вершин
  bd.CPUAccessFlags = 0;
  return bd;
}
Chunk::~Chunk() {
}