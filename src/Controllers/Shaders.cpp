#include "Shaders.hh"

#define ERRMSG(text)                                               \
  MessageBox(NULL, text, L"������ ��������", MB_OK | MB_ICONHAND); \
  exit(-1);

#define H_ERRMSG(hr, text)                                           \
  if (FAILED(hr)) {                                                  \
    MessageBox(NULL, text, L"������ ��������", MB_OK | MB_ICONHAND); \
    exit(-1);                                                        \
  }
#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"������ ��������", MB_OK | MB_ICONEXCLAMATION);

Shaders::Shaders()
    : versions({"s_4_0", "s_4_1", "s_5_0"}), hlsl_types({{"int", 4}, {"dword", 4}, {"float", 4}, {"half", 2}, {"bool", 1}}) {
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
  return pShader;
}

int Shaders::compile_file(stlcwstr& filepath, stlcstr& shader_version) {
  auto supported_version = std::find(versions.begin(), versions.end(), shader_version);
  if (supported_version == versions.end())
    std::runtime_error e("����������� ������ �������.");

  std::ifstream src(filepath.c_str());
  if (!src) {
    ERRMSG(L"������������ ������-���� � ������� ��������.");
  }
  auto doc = parse_hlsl_file(src);

  //    hr = D3DX11CompileFromFileW(
  //    filename.c_str(), NULL, NULL, entryPoint.c_str(), shaderModel.c_str(),
  //    shaderFlags, 0, NULL, o_blob, &errInfo, NULL);
  return 0;
}

std::vector<stlstr> Shaders::parse_hlsl_file(std::ifstream& src) {
  std::vector<stlstr> read;
  const_buffers cb_umap;

  separator delim("{\n :)(");
  stlcstr file_string = {std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>()};
  tokenizer tok(file_string, delim);

  sstream ss("������ ��������: ");
  for (auto it = tok.begin(); it != tok.end(); ++it) {
    // ss << *it << "|\n";
    if (*it == "cbuffer") {
      cb_name name = *(++it);
      ++it; // ����� "register"
      cb_id id((++it)->substr(1));

      ss << name << " " << id.num << '\n';
      // ss << name << '+';
    }
    // ss << *it << '+';
  }

  COUTNL(ss);

  sstream separated({std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>()});

  stlstr str;
  cb_name name;
  cb_id id;

  while (separated >> str) {
    if (str == "cbuffer") {
    }
  }
  return read;
}

HRESULT Shaders::compileFromFile(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel, ID3DBlob** o_blob) {
  HRESULT hr = S_OK;
  const DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  ID3DBlob* errInfo; // ������ ������
  hr = D3DX11CompileFromFileW(
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

ID3D11InputLayout* Shaders::getVertexLayout(stlcstr& entryPoint) {
  auto it = vertexLayouts.find(entryPoint);
  return ((it != vertexLayouts.end()) ? (it->second) : (NULL));
}