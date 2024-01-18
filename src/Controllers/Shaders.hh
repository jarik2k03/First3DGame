#pragma once
#ifndef SHADERS_HH
#define SHADERS_HH
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <Controllers/VShader.hh>
#include <_Commons/vertex.h>
#include <_Commons/modtype.h>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string/erase.hpp>

#include <fstream>
#include <d3dcompiler.h>
#include <map>
#include <unordered_map>
#include <vector>

using hlsl_datatypes = std::unordered_map<stlstr, int>;
// stlstr - название типа; int - размер; 

using struct_name = stlstr;
using struct_size = unsigned int;

using cb_name = stlstr;
using cb_size = unsigned int;
using cb_id = mod_int;
using const_buffers = std::unordered_map<cb_name, std::pair<cb_id, cb_size>>;

using fnc_name = stlstr;
using abstract_shader = std::unique_ptr<AShader>;
using compiled_shaders = std::unordered_map<fnc_name, abstract_shader>;

using filename = stlwstr;

using separator = boost::char_separator<char>;
using tokenizer = boost::tokenizer<separator>;

using token_iterator_t = boost::token_iterator<separator, stlstr::const_iterator, stlstr>;

class Shaders {
 public:
  Shaders();
  ~Shaders();
  ID3D11InputLayout* getVertexLayout(stlcstr& entryPoint);
  ID3D11VertexShader* addVertexShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel = "vs_5_0");
  ID3D11PixelShader* addPixelShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel = "ps_5_0");
  int compile_file(stlcwstr& filepath, stlcstr& shader_version = "s_5_0");


 protected:
  std::unordered_map<stlstr, ID3D11InputLayout*> vertexLayouts; // набор вершинных макетов
  std::unordered_map<filename, std::pair<const_buffers, compiled_shaders>> file;
  std::vector<stlstr> versions;
  hlsl_datatypes hlsl_types;
  HRESULT compileFromFile(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel, ID3DBlob** blobOut);

  std::vector<stlstr> parse_hlsl_file_(std::ifstream& src);

  unsigned int init_hlsl_struct__(token_iterator_t&& word);

  void calc_offset_n(int type_size, token_iterator_t&& word, int&& offset, int&& n);

  int calc_type_size___(const stlcstr& type);

  int calc_array_size___(const int type_size, const int length);

};


void remove_hlsl_comments(stlstr&& filedata);
#endif