#pragma once
#ifndef SHADERS_HH
#define SHADERS_HH
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <_Commons/modtype.h>
#include <_Commons/vertex.h>
#include <Controllers/VShader.hh>
#include <Controllers/Shaders/ConstBuffer.hh>
#include <Controllers/Shaders/CB_Unit.hh>

#include <boost/algorithm/string/erase.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/tokenizer.hpp>
#include <boost/variant/variant.hpp>

#include <d3dcompiler.h>
#include <fstream>
#include <map>
#include <unordered_map>
#include <vector>

using hlsl_datatypes = std::unordered_map<stlstr, int>;
// stlstr - название типа; int - размер;

using struct_name = stlstr;
using struct_size = unsigned int;




using fnc_name = stlstr;
using abstract_shader = std::unique_ptr<AShader>;
using compiled_shaders = std::unordered_map<fnc_name, abstract_shader>;

using filename = stlwstr;

using separator = boost::char_separator<char>;
using tokenizer = boost::tokenizer<separator>;

using token_iterator_t = boost::token_iterator<separator, stlstr::const_iterator, stlstr>;

struct variable {
  stlstr type, var_name;
  std::size_t array_size, offset;
  variable(stlcstr& t, stlcstr& v, std::size_t s, std::size_t of) : type(t), var_name(v), array_size(s), offset(of) {
  }
};
using structure = std::vector<variable>;

class shader_blob {
 private:
  const_buffers cb;
  compiled_shaders shaders;

 public:
  shader_blob(const const_buffers& cbs, const compiled_shaders& sh) : cb(cbs), shaders(sh) {
  }
  const ConstBuffer& operator()(stlcstr& cb_name) const {
    const auto& it = cb.find(cb_name);
    if (it == cb.end()) 
      std::runtime_error e("Ќе найден константный буфер");
    return it->second;
  }
};

class Shaders {
 public:
  Shaders();
  ~Shaders();
  ID3D11InputLayout* getVertexLayout(stlcstr& entryPoint);
  ID3D11VertexShader* addVertexShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel = "vs_5_0");
  ID3D11PixelShader* addPixelShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel = "ps_5_0");
  shader_blob compile_file(stlcwstr& filepath, stlcstr& shader_version = "s_5_0");

 protected:
  std::unordered_map<stlstr, ID3D11InputLayout*> vertexLayouts; // набор вершинных макетов
  std::unordered_map<filename, std::pair<const_buffers, compiled_shaders>> file;

  std::vector<stlstr> versions;
  hlsl_datatypes hlsl_types;
  HRESULT compileFromFile(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel, ID3DBlob** blobOut);

  std::vector<stlstr> parse_hlsl_file_(std::ifstream& src);

  std::size_t init_hlsl_struct(std::vector<stlstr>::iterator&& word, bool in_cb = false);
  std::size_t init_temp_hlsl_struct(std::vector<stlstr>::iterator&& word);

  std::size_t init_hlsl_cbuffer__(std::vector<stlstr>::iterator&& word);

  void calc_variable(int type_size, std::vector<stlstr>::iterator&& word, int&& offset, int&& n);
  int calc_type_size___(const stlcstr& type);
  int calc_array_size___(const int type_size, const int length);
};

void remove_hlsl_comments(stlstr&& filedata);
#endif