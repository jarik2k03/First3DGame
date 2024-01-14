#include "Shaders.hh"

#define PACK_SIZE 16

#define ERRMSG(text)                                               \
  MessageBoxA(NULL, text, "������ ��������", MB_OK | MB_ICONHAND); \
  exit(-1);

#define WARNMSG(text) MessageBoxA(NULL, text, "������ ��������", MB_OK | MB_ICONEXCLAMATION);

#define H_ERRMSG(hr, text)                                           \
  if (FAILED(hr)) {                                                  \
    MessageBox(NULL, text, L"������ ��������", MB_OK | MB_ICONHAND); \
    exit(-1);                                                        \
  }
#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"������ ��������", MB_OK | MB_ICONEXCLAMATION);
Shaders::Shaders()
    : versions({"s_4_0", "s_4_1", "s_5_0"}),
      hlsl_types({{"int", 4}, {"DWORD", 4}, {"half", 2}, {"bool", 1}, {"matrix", 64}, {"float", 4}}) {
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
    ERRMSG("������������ ������-���� � ������� ��������.");
  }
  auto doc = parse_hlsl_file_(src);

  //    hr = D3DX11CompileFromFileW(
  //    filename.c_str(), NULL, NULL, entryPoint.c_str(), shaderModel.c_str(),
  //    shaderFlags, 0, NULL, o_blob, &errInfo, NULL);
  return 0;
}

std::vector<stlstr> Shaders::parse_hlsl_file_(std::ifstream& src) {
  std::vector<stlstr> read;
  // const_buffers cb_umap;

  separator delim(" \n", ",:;(){}[]");
  stlstr file_string = {std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>()};
  remove_hlsl_comments(std::move(file_string));

  tokenizer tok(file_string, delim);

  for (auto it = tok.begin(); it != tok.end(); ++it) {
    if (*it == "struct") {
      init_hlsl_struct__(std::move(it));
      break;
    }
  }

  return read;
}

unsigned int Shaders::init_hlsl_struct__(boost::token_iterator<separator, stlstr::const_iterator, stlstr>&& word) {
  sstream ss;
  stlcstr name = *(++word);
  ++word; // {
  int offset = 0, n = 0;
  while (*(++word) != "}") {
    if (*word == "struct") // ������: struct Temp tmp; <=> Temp tmp;
      ++word;
    const int type_size = calc_type_size___(*word);
  repeat:
    if (offset + type_size > n * PACK_SIZE) {
      offset = n * PACK_SIZE + type_size;
      n += (type_size < PACK_SIZE) ? 1 : (type_size / PACK_SIZE);
    } else {
      offset += type_size;
    }
    *(++word); // �������� ���������� [optional]
    if (*(++word) == ",") //
      goto repeat;
  }
  hlsl_types.insert({name, offset}); // ��������� ����������� ���������

  ss << "offset: " << offset;
  COUTNL(ss);

  return offset;
}

void remove_hlsl_comments(stlstr&& filedata) {
  bool c_diap = false, c_line = false;
  const auto func = [&](const char& cur) {
    const char next = filedata.at(&cur - &*filedata.begin() + 1); // ��. ����� (�������� ����������)
    const char prev = filedata.at(&cur - &*filedata.begin() - 1); // ����. ����� (�������� ����������)
    if (cur == '/' && next == '*')
      return static_cast<int>(c_diap = true);
    if (prev == '*' && cur == '/')
      return static_cast<int>(~(c_diap = false)); // ������� '/' � ��������� ��������
    if (cur == '/' && next == '/')
      return static_cast<int>(c_line = true);
    if (cur == '\n')
      return static_cast<int>(c_line = false); // ����� "//"-����������� (c_line)
    return c_line | c_diap;
  };
  auto it = std::remove_if(filedata.begin() + 1, filedata.end() - 1, func);
  filedata.erase(it, filedata.end()); 
  sstream ss(filedata);
  COUTNL(ss);
}

int Shaders::calc_type_size___(stlcstr& type) {
  auto f = std::find_if(type.begin(), type.end(), ::isdigit);
  stlcstr raw_type(type.begin(), f), value(f, type.end());
  auto t = hlsl_types.find(raw_type); // Temp
  if (value.empty())
    return t->second;

  int row_size = value.at(0) - '0'; // char to int
  if (value.size() == 1) // float1, half2, int3 ...
    return t->second * row_size;

  int col_size = value.at(2) - '0'; // char to int
  if (value.size() == 3) // float4x4, float1x3, int2x2 ...
    return t->second * 4 * col_size;
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