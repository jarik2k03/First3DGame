#include "Chunk.hh"
#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Chunk::Chunk(
    int length,
    const XMFLOAT3& xyz,
    const std::pair<stlwstr, model_buffer>& b,
    ID3D11VertexShader* v,
    ID3D11PixelShader* p,
    ID3D11InputLayout* l)
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
      p_shader(p),
      layout_(l) {
  auto* blocks = chunk.get();
  int volume = length * length * length;
  int area = length * length;

  for (int i = 0; i < volume; ++i) {
    if (blocks[i].is_shown())
      shown_blocks.emplace_back(i);
  }

   auto bd = set_const_buf(sizeof(Block) * volume, D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
  D3D11_SUBRESOURCE_DATA d = {0};
  d.pSysMem = shown_blocks.data();
   auto hr = Device::d3d->CreateBuffer(&bd, &d, &chunk_buf_);
  Device::ic->VSSetConstantBuffers(4, 1, &chunk_buf_);
  H_WARNMSG(hr, L"Ошибка инициализации константного буфера");

  auto bd2 = set_const_buf(sizeof(CounterBuf) * 4, D3D11_USAGE_DEFAULT);
  hr = Device::d3d->CreateBuffer(&bd2, NULL, &counter_buf_);
  H_WARNMSG(hr, L"Ошибка инициализации константного буфера");
}

void Chunk::update_render() {
  unsigned int stride = sizeof(AdvVertex);
  unsigned int offset = 0;
  // Device::ic->UpdateSubresource(chunk_buf_, 0, NULL, shown_blocks.data(), 0, 0);
  
  Device::ic->IASetInputLayout(layout_);
  Device::ic->IASetVertexBuffers(0, 1, &vertex_buf_, &stride, &offset);
  Device::ic->IASetIndexBuffer(index_buf_, DXGI_FORMAT_R16_UINT, 0);
  Device::ic->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  Device::ic->VSSetShader(v_shader, NULL, 0);
  Device::ic->PSSetShader(p_shader, NULL, 0);
  Device::ic->PSSetShaderResources(0, 1, &textureSRV);
  Device::ic->PSSetSamplers(0, 1, &sampler_buf_);

  int area = length * length;
  int volume = length * length * length;
  auto* blocks = chunk.get();

  for (size_t i = 0; i < shown_blocks.size(); i++) {
    CounterBuf b(i, length, area);
    Device::ic->UpdateSubresource(counter_buf_, 0, NULL, &b, 0, 0);
    Device::ic->VSSetConstantBuffers(5, 1, &counter_buf_);
    Device::ic->DrawIndexed(36, 0, 0);
  }
}

D3D11_BUFFER_DESC Chunk::set_const_buf(int byte_width, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = usage;
  bd.ByteWidth = byte_width;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // - тип буфера буфер вершин
  bd.CPUAccessFlags = cpu_access;
  return bd;
}
Chunk::~Chunk() {
}