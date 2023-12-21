#pragma once
#ifndef MODELER_HH
#define MODELER_HH

#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <_Commons/vertex.h>
#include <unordered_map>
#include <vector>
#include <algorithm>

using vertices = std::vector<AdvVertex>;
using indices = std::vector<WORD>;

vertices init_position();
indices init_indexed();

struct model_buffer {
  ID3D11Buffer* vertex;
  ID3D11Buffer* index;
  ID3D11SamplerState* sample;
  ID3D11ShaderResourceView* texture;
  WORD idx_len;
};
using model_buffers = std::unordered_map<stlwstr, model_buffer>;


class Modeler {
 protected:
  model_buffers saved_models;

  D3D11_SUBRESOURCE_DATA set_res_data(void* vert);
  D3D11_BUFFER_DESC set_buffer_desc(int byteWidth, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
  D3D11_SAMPLER_DESC set_sampler_desc(
      D3D11_COMPARISON_FUNC cmp = D3D11_COMPARISON_NEVER,
      D3D11_FILTER mip = D3D11_FILTER_MIN_MAG_MIP_LINEAR,
      float minLOD = 0.0f,
      float maxLOD = D3D11_FLOAT32_MAX);
 public:
  void create_model(stlcwstr& name, const vertices& vertex_rule, const indices& index_rule);
  model_buffers::const_iterator get_model(stlcwstr& name) const;
  void remove_model(stlcwstr& name);
};




#endif // !MODELER_HXX