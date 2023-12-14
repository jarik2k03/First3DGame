#include "Cube.h"

#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"������ ���������", MB_OK | MB_ICONEXCLAMATION);

Cube::Cube(const XMFLOAT3& xyz, const model_buffer& b, ID3D11VertexShader* v, ID3D11PixelShader* p)
    : vShader(v),
      pShader(p),
      pos{-xyz.x, -xyz.y, -xyz.z},
      vertex_buf_(b.vertex),
      index_buf_(b.index),
      textureSRV(b.texture),
      sampler_buf_(b.sample) {

  auto bd = set_const_buf(sizeof(XMMATRIX) * 3, D3D11_USAGE_DEFAULT);
  auto hr = Device::d3d->CreateBuffer(&bd, NULL, &const_buf_);
  H_WARNMSG(hr, L"������ ������������� ������������ ������");
}

void Cube::update_state(XMMATRIX& view, XMMATRIX& proj) {
  XMMATRIX world_position = XMMatrixTranslation(pos.x, pos.y, pos.z);
  ConstantBuffer cb;
  cb.world = XMMatrixTranspose(world_position);
  cb.view = XMMatrixTranspose(view);
  cb.proj = XMMatrixTranspose(proj);
  Device::ic->UpdateSubresource(const_buf_, 0, NULL, &cb, 0, 0);
}


D3D11_BUFFER_DESC Cube::set_const_buf(int byte_width, D3D11_USAGE usage) {
  D3D11_BUFFER_DESC bd;
  ZeroMemory(&bd, sizeof(bd));
  bd.Usage = usage;
  bd.ByteWidth = byte_width;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // - ��� ������ ����� ������
  bd.CPUAccessFlags = 0;
  return bd;
}

void Cube::render() {
  unsigned int stride = sizeof(AdvVertex);
  unsigned int offset = 0;
  // ������ ���� ������ � InputAssembler'�
  Device::ic->IASetVertexBuffers(0, 1, &vertex_buf_, &stride, &offset);
  Device::ic->IASetIndexBuffer(index_buf_, DXGI_FORMAT_R16_UINT, 0);
  Device::ic->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  // ������ �������
  Device::ic->VSSetShader(vShader, NULL, 0);
  Device::ic->PSSetShader(pShader, NULL, 0);
  // �������� ������ � ������
  Device::ic->VSSetConstantBuffers(0, 1, &const_buf_);
  // �������� �������� � ������
  Device::ic->PSSetShaderResources(0, 1, &textureSRV);
  Device::ic->PSSetSamplers(0, 1, &sampler_buf_);
  // ������� ��������� ����������� �� ������
  Device::ic->DrawIndexed(36, 0, 0);
}
