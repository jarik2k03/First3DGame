#include "Chunk.hh"
#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка геометрии", MB_OK | MB_ICONEXCLAMATION);

Chunk::Chunk(
    int length,
    const XMFLOAT3& xyz,
    const std::pair<stlwstr, model_buffer>& b,
    const shader_blob& compiled,
    ID3D11VertexShader* v,
    ID3D11PixelShader* p,
    ID3D11InputLayout* l)
    : chunk(new Block[length * length * length], [](Block* chunk) { delete[] chunk; }),
      model(b),
      pos{-xyz.x, -xyz.y, -xyz.z},
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



  CB_Unit chunk_buf_(compiled("ChunkPosBuffer"), shown_blocks.data(), D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
  Device::ic->VSSetConstantBuffers(chunk_buf_.id(), 1, &*chunk_buf_);
  CB_Unit counter_buf_(compiled("Counter"), D3D11_USAGE_DYNAMIC, D3D11_CPU_ACCESS_WRITE);
}

void Chunk::update_render() {
  unsigned int stride = sizeof(AdvVertex);
  unsigned int offset = 0;
  // Device::ic->UpdateSubresource(chunk_buf_, 0, NULL, shown_blocks.data(), 0, 0);

  Device::ic->IASetInputLayout(layout_);
  Device::ic->IASetVertexBuffers(0, 1, &model.second.vertex, &stride, &offset);
  Device::ic->IASetIndexBuffer(model.second.index, DXGI_FORMAT_R16_UINT, 0);
  Device::ic->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  Device::ic->VSSetShader(v_shader, NULL, 0);
  Device::ic->PSSetShader(p_shader, NULL, 0);
  Device::ic->PSSetShaderResources(0, 1, &model.second.texture);
  Device::ic->PSSetSamplers(0, 1, &model.second.sample);

  int area = length * length;
  int volume = length * length * length;
  auto* blocks = chunk.get();

  for (size_t i = 0; i < shown_blocks.size(); i++) {
    CounterBuf b(i, length, area);
    Device::ic->UpdateSubresource(*counter_buf_, 0, NULL, &b, 0, 0);
    Device::ic->VSSetConstantBuffers(5, 1, &counter_buf_);
    Device::ic->DrawIndexed(36, 0, 0);
  }
}

Chunk::~Chunk() {
}