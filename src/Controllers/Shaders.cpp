#include "Shaders.h"

#define H_ERRMSG(hr, text)                                           \
  if (FAILED(hr)) {                                                  \
    MessageBox(NULL, text, L"������ ��������", MB_OK | MB_ICONHAND); \
    exit(-1);                                                        \
  }
#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"������ ��������", MB_OK | MB_ICONEXCLAMATION);

Shaders::Shaders() {
}

Shaders::~Shaders() {
}
ID3D11VertexShader* Shaders::addVertexShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel) {
  HRESULT hr = S_OK;
  ID3DBlob* curBlob = NULL;
  hr = compileFromFile(filename, entryPoint, shaderModel, &curBlob); // entryPoint - �������, ��������� � fx
  H_ERRMSG(hr, L"���������� �������������� ���� FX. ����������, �������������� ���������.");
  // ������ ������ �� ����������������� �������
  ID3D11VertexShader* vShader;
  hr = Device::d3d->CreateVertexShader(curBlob->GetBufferPointer(), curBlob->GetBufferSize(), NULL, &vShader);
  H_ERRMSG(hr, L"�� ������� ������� ��������� ������.");
   
  int offset = 0;
  D3D11_INPUT_ELEMENT_DESC layout[3] = {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}};
  // ������� ����� ������ vertexLayout
  ID3D11InputLayout* vLayout;
  hr = Device::d3d->CreateInputLayout(layout, ARRAYSIZE(layout), curBlob->GetBufferPointer(), curBlob->GetBufferSize(), &vLayout);
  curBlob->Release();
  H_ERRMSG(hr, L"�� �������� ����� ������");
  Device::ic->IASetInputLayout(vLayout);

  vertexShaders.insert(std::make_pair(entryPoint, vShader)); // ������ ���������� ������� � ����� ������
  vertexLayouts.insert(std::make_pair(entryPoint, vLayout)); // ������ ���������� ������ � ����� ������
  return vShader;
}

ID3D11PixelShader* Shaders::addPixelShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel) {
  HRESULT hr = S_OK;
  ID3DBlob* curBlob = NULL;
  hr = compileFromFile(filename, entryPoint, shaderModel, &curBlob); // entryPoint - �������, ��������� � fx
  H_ERRMSG(hr, L"���������� �������������� ���� FX. ����������, �������������� ���������.");
  // ������ ������ �� ����������������� �������
  ID3D11PixelShader* pShader;
  hr = Device::d3d->CreatePixelShader(curBlob->GetBufferPointer(), curBlob->GetBufferSize(), NULL, &pShader);
  H_ERRMSG(hr, L"�� ������� ������� ���������� ������.");
  curBlob->Release();

  pixelShaders.insert(std::make_pair(entryPoint, pShader)); // ������ ����������� ������� � ����� ������
  return pShader;
}

HRESULT Shaders::compileFromFile(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel, ID3DBlob** o_blob) {
  HRESULT hr = S_OK;
  DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  ID3DBlob* errInfo; // ������ ������
  hr = D3DX11CompileFromFile(
      filename.c_str(), NULL, NULL, entryPoint.c_str(), shaderModel.c_str(), shaderFlags, 0, NULL, o_blob, &errInfo, NULL);

  if (FAILED(hr)) {
    MessageBoxA(NULL, (LPCSTR)errInfo->GetBufferPointer(), (LPCSTR) "������ ��������", MB_OK | MB_ICONEXCLAMATION);
    if (errInfo)
      errInfo->Release();
    exit(-2);
    return hr;
  }
  if (errInfo)
    errInfo->Release();
  return S_OK;
}

ID3D11VertexShader* Shaders::getVertexShader(stlstr& entryPoint) {
  auto it = vertexShaders.find(entryPoint);
  return ((it != vertexShaders.end()) ? (it->second) : (NULL));
}

ID3D11InputLayout* Shaders::getVertexLayout(stlstr& entryPoint) {
  auto it = vertexLayouts.find(entryPoint);
  return ((it != vertexLayouts.end()) ? (it->second) : (NULL));
}

ID3D11PixelShader* Shaders::getPixelShader(stlstr& entryPoint) {
  auto it = pixelShaders.find(entryPoint);
  return ((it != pixelShaders.end()) ? (it->second) : (NULL));
}