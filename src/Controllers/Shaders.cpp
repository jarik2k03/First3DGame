#include "Shaders.hh"
#include <_Commons/str_switch.h>

#define PACK_SIZE 16
#define FAIL_PASRE_STRUCT 0

#define ERRMSG(text)                                               \
  MessageBoxA(NULL, text, "Ошибка шейдеров", MB_OK | MB_ICONHAND); \
  exit(-1);

#define WARNMSG(text) MessageBoxA(NULL, text, "Ошибка шейдеров", MB_OK | MB_ICONEXCLAMATION);

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
      hlsl_types({{"int", 4}, {"DWORD", 4}, {"half", 4}, {"bool", 4}, {"matrix", 64}, {"float", 4}}) {
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

shader_blob Shaders::compile_file(stlcwstr& filepath, stlcstr& shader_version) {
  auto supported_version = std::find(versions.begin(), versions.end(), shader_version);
  if (supported_version == versions.end())
    std::runtime_error e("Недоступная версия шейдера.");

  std::ifstream src(filepath.c_str());
  if (!src) {
    ERRMSG("Отстутствует шейдер-файл в текущем каталоге.");
  }
  auto doc = parse_hlsl_file_(src);

  //    hr = D3DX11CompileFromFileW(
  //    filename.c_str(), NULL, NULL, entryPoint.c_str(), shaderModel.c_str(),
  //    shaderFlags, 0, NULL, o_blob, &errInfo, NULL);
  return shader_blob(
      {
          {"Counter", ConstBuffer(5, 16)},
          {"ChunkPosBuffer", ConstBuffer(4, 65660)},
          {"PositionBuffer", ConstBuffer(0, 64)},
          {"LightBuffer", ConstBuffer(1, 48)},
          {"ViewBuffer", ConstBuffer(2, 64)},
          {"ProjBuffer", ConstBuffer(3, 64)},
          {"PositionBuffer", ConstBuffer(0, 64)},
          {"temp", ConstBuffer(12, 636)},
          {"tem", ConstBuffer(9, 124)},
      },
      {});
}

std::vector<stlstr> Shaders::parse_hlsl_file_(std::ifstream& src) {
  // const_buffers cb_umap;
  sstream ss;
  separator delim(" \n", ",:;(){}[]");
  stlstr file_string = {std::istreambuf_iterator<char>(src), std::istreambuf_iterator<char>()};
  remove_hlsl_comments(std::move(file_string));

  tokenizer tok(file_string, delim);
  std::vector<stlstr> data(tok.begin(), tok.end());
  for (auto it = data.begin(); it != data.end(); ++it) {
    SWITCH(*it) {
      CASE("struct") : init_hlsl_struct(std::move(it));
      break;
      // CASE("cbuffer") : ss << "cbuffer: " << init_hlsl_cbuffer__(std::move(++it)) << " ";
      // break;
    }
  }

  COUTNL(ss);
  return data;
}

std::size_t Shaders::init_hlsl_struct(std::vector<stlstr>::iterator&& word, bool in_cb) {
  int offset = 0, n = 0, type_size = 0;
  stlstr name = *(++word); // struct -> "name" || "{"
  name = (name == "{") ? "<unnamed>" : *(word++);

  while (*(++word) != "}") {
    if (*(word += 2) == "struct")
      if (*(word + 1) == "{") { // обнаруживаем временную структуру
        type_size = (*(word + 1) == "{") ? init_temp_hlsl_struct(std::move(word)) : calc_type_size___(*(++word));
      }
    do { // как минимум одна переменная должна быть объявлена
      calc_variable(type_size, std::move(word), std::move(offset), std::move(n));
    } while (*(word) == ",");
  }
  if (!in_cb)
    return offset;
  int size = offset;
  while (*word != ";") { // вычисление после объявления структуры
    calc_variable(offset, std::move(word), std::move(size), std::move(n));
  }
  word += 1;
  return size;
}

std::size_t Shaders::init_temp_hlsl_struct(std::vector<stlstr>::iterator&& word) {
  int offset = 0, n = 0, type_size = 0;
  while (*(++word) != "}") {
    if (*(word += 2) == "struct") {
      type_size = (*(word + 1) == "{") ? init_temp_hlsl_struct(std::move(word)) : calc_type_size___(*(++word));
    }
    do { // как минимум одна переменная должна быть объявлена
      calc_variable(type_size, std::move(word), std::move(offset), std::move(n));
    } while (*(word) == ",");
  }
  int size = offset;
  while (*word != ";") { // вычисление после объявления структуры
    calc_variable(offset, std::move(word), std::move(size), std::move(n));
  }
  word += 1;
  return size;
}

std::size_t Shaders::init_hlsl_cbuffer__(std::vector<stlstr>::iterator&& word) {
  sstream ss;
  stlcstr name = *(word);
  ++word, ++word, ++word; // "Name" -> ":" -> "register" -> "("
  int buf_id = std::stoi(stlcstr(*(++word), 1)); // "b1"
  ss << "cbuffer id: " << buf_id << '\n';
  ++word, ++word; // ")" -> "{"
  int offset = 0, n = 0, tempstruct_size = FAIL_PASRE_STRUCT;
  while (*(++word) != "}") {
    if (*word == "struct")
      if ((tempstruct_size = init_hlsl_struct(std::move((word)))) == FAIL_PASRE_STRUCT) {
        ++word; // случай: struct Temp tmp; <=> Temp tmp;
      }
    int type_size = (tempstruct_size == FAIL_PASRE_STRUCT) ? calc_type_size___(*(word)) : tempstruct_size;
    ss << "  type_size: " << type_size << '\n';
    do { // как минимум одна переменная должна быть объявлена
      calc_variable(type_size, std::move(word), std::move(offset), std::move(n));
    } while (*(word) == ",");
  }
  ss << "cbuffer size: " << offset << '\n';
  COUTNL(ss);
  return offset;
}

void Shaders::calc_variable(int type_size, std::vector<stlstr>::iterator&& word, int&& offset, int&& n) {
  int arr_length = 1, is_array_type = false;
  word += 2; // "," -> "varname" -> ("[") или (",")
  if (*(word) == "[") {
    arr_length = std::stoi(*(++word)); // длина массива "[" -> "ARR_SIZE"
    is_array_type = true, word += 2; // "ARR_SIZE" -> "]" -> ","
  }
  if (*word == ":") {
    word += 2;
  }
  type_size = calc_array_size___(type_size, arr_length);
  if (offset + type_size > n * PACK_SIZE || is_array_type == true) {
    offset = n * PACK_SIZE + type_size;
    n += std::ceilf((float)type_size / PACK_SIZE);
  } else {
    offset += type_size;
  }
}

int Shaders::calc_type_size___(stlcstr& type) {
  auto f = std::find_if(type.begin(), type.end(), ::isdigit);
  stlcstr raw_type(type.begin(), f), value(f, type.end());
  auto t = hlsl_types.find(raw_type); // Temp
  if (t == hlsl_types.end())
    return -1;
  if (value.empty())
    return t->second;
  int row_size = value.at(0) - '0'; // char to int
  if (value.size() == 1) // float1, half2, int3 ...
    return t->second * row_size;
  int col_size = value.at(2) - '0'; // char to int
  int aligned_size = t->second * 4 * (col_size - 1);
  if (value.size() == 3) // float4x4, float1x3, int2x2 ...
    return aligned_size + (t->second * row_size);
}
int Shaders::calc_array_size___(const int type_size, const int length) {
  int aligned_size = std::ceilf((float)type_size / PACK_SIZE) * PACK_SIZE * (length - 1);
  return aligned_size + type_size;
}

void remove_hlsl_comments(stlstr&& filedata) {
  bool c_diap = false, c_line = false;
  const auto has_comment = [&](const char& cur) {
    const size_t cur_index = &cur - &*filedata.begin(); // индекс cur элемента (адресная арифметика)
    const char next = filedata.at(cur_index + 1); // сл. буква
    const char prev = (cur_index != 0) ? filedata.at(cur_index - 1) : ' '; // пред. буква
    if (cur == '/' && next == '*')
      return static_cast<int>(c_diap = true);
    if (prev == '*' && cur == '/')
      return static_cast<int>(~(c_diap = false)); // стираем '/' и запрещаем стирание
    if (cur == '/' && next == '/')
      return static_cast<int>(c_line = true);
    if (cur == '\n')
      return static_cast<int>(c_line = false); // конец "//"-комментария (c_line)
    return c_line | c_diap;
  };
  auto it = std::remove_if(filedata.begin(), filedata.end() - 1, has_comment);
  filedata.erase(it, filedata.end());
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