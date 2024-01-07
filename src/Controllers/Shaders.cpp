#include "Shaders.hh"

#define PACK_SIZE 16

#define ERRMSG(text)                                               \
  MessageBox(NULL, text, L"Ошибка шейдеров", MB_OK | MB_ICONHAND); \
  exit(-1);

#define H_ERRMSG(hr, text)                                           \
  if (FAILED(hr)) {                                                  \
    MessageBox(NULL, text, L"Ошибка шейдеров", MB_OK | MB_ICONHAND); \
    exit(-1);                                                        \
  }
#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка шейдеров", MB_OK | MB_ICONEXCLAMATION);

Shaders::Shaders()
    : versions({"s_4_0", "s_4_1", "s_5_0"}),
      hlsl_types({{"int", 4}, {"dword", 4}, {"float", 4}, {"half", 2}, {"bool", 1}, {"matrix", 64}, {"float2", 8}, {"float3", 12}}) {
}

Shaders::~Shaders() {
}
ID3D11VertexShader* Shaders::addVertexShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel) {
  HRESULT hr = S_OK;
  ID3DBlob* curBlob = NULL;
  hr = compileFromFile(filename, entryPoint, shaderModel, &curBlob); // entryPoint - функция, описанная в fx
  H_ERRMSG(hr, L"Невозможно скомпилировать файл FX. Пожалуйста, переустановите программу.");
  // создаём шейдер из скомпилированного объекта
  ID3D11VertexShader* vShader;
  hr = Device::d3d->CreateVertexShader(curBlob->GetBufferPointer(), curBlob->GetBufferSize(), NULL, &vShader);
  H_ERRMSG(hr, L"Не удалось создать вершинный шейдер.");

  int offset = 0;
  D3D11_INPUT_ELEMENT_DESC layout[3] = {
      {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
      {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0}};
  // создали макет вершин vertexLayout
  ID3D11InputLayout* vLayout;
  hr = Device::d3d->CreateInputLayout(layout, ARRAYSIZE(layout), curBlob->GetBufferPointer(), curBlob->GetBufferSize(), &vLayout);
  curBlob->Release();
  H_ERRMSG(hr, L"Не создаётся макет вершин");

  vertexLayouts.insert(std::make_pair(entryPoint, vLayout)); // запись вершинного макета в ассоц массив
  return vShader;
}

ID3D11PixelShader* Shaders::addPixelShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel) {
  HRESULT hr = S_OK;
  ID3DBlob* curBlob = NULL;
  hr = compileFromFile(filename, entryPoint, shaderModel, &curBlob); // entryPoint - функция, описанная в fx
  H_ERRMSG(hr, L"Невозможно скомпилировать файл FX. Пожалуйста, переустановите программу.");
  // создаём шейдер из скомпилированного объекта
  ID3D11PixelShader* pShader;
  hr = Device::d3d->CreatePixelShader(curBlob->GetBufferPointer(), curBlob->GetBufferSize(), NULL, &pShader);
  H_ERRMSG(hr, L"Не удалось создать пиксельный шейдер.");
  curBlob->Release();
  return pShader;
}

int Shaders::compile_file(stlcwstr& filepath, stlcstr& shader_version) {
  auto supported_version = std::find(versions.begin(), versions.end(), shader_version);
  if (supported_version == versions.end())
    std::runtime_error e("Недоступная версия шейдера.");

  std::ifstream src(filepath.c_str());
  if (!src) {
    ERRMSG(L"Отстутствует шейдер-файл в текущем каталоге.");
  }
  auto doc = parse_hlsl_file_(src);

  //    hr = D3DX11CompileFromFileW(
  //    filename.c_str(), NULL, NULL, entryPoint.c_str(), shaderModel.c_str(),
  //    shaderFlags, 0, NULL, o_blob, &errInfo, NULL);
  return 0;
}

std::vector<stlstr> Shaders::parse_hlsl_file_(std::ifstream& src) {
  std::vector<stlstr> read;
  //const_buffers cb_umap;

  separator delim(" \n", ":;(){}[]");
  stlcstr file_string = {std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>()};
  tokenizer tok(file_string, delim);

  sstream ss;
  for (auto it = tok.begin(); it != tok.end(); ++it) {
    ss << *it << " ";
    if (*it == "struct") {
      init_hlsl_struct__(std::move(it));
    }
  }
  COUTNL(ss);

  return read;
}

unsigned int Shaders::init_hlsl_struct__(boost::token_iterator<separator, stlstr::const_iterator, stlstr>&& word) {
  stlcstr name = *(++word);
  int free_space = PACK_SIZE, fullsize = 0;
  while (word->at(0) != '}') {
    if (*word == "struct") {
      ++word; // случай struct Temp tmp; <=> Temp tmp;
    }
    //int type_size = hlsl_types.find(*(++word))->second; // Temp
    //free_space -= type_size;
    if (free_space <= 0) {
      //free_space = -free_space % type_size, fullsize += type_size;
    }

    *(++word); // название переменной [optional] // tmp
    *(++word); // ;
  }

  return fullsize;
}

HRESULT Shaders::compileFromFile(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel, ID3DBlob** o_blob) {
  HRESULT hr = S_OK;
  const DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  ID3DBlob* errInfo; // список ошибок
  hr = D3DX11CompileFromFileW(
      filename.c_str(), NULL, NULL, entryPoint.c_str(), shaderModel.c_str(), shaderFlags, 0, NULL, o_blob, &errInfo, NULL);

  if (FAILED(hr)) {
    MessageBoxA(NULL, (LPCSTR)errInfo->GetBufferPointer(), (LPCSTR) "Ошибка шейдеров", MB_OK | MB_ICONEXCLAMATION);
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