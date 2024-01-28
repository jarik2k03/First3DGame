#pragma once
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <memory>
#include <unordered_map>

using cb_name = stlstr;
using cb_size = unsigned int;
using cb_id = unsigned int;

using const_buffers = std::unordered_map<cb_name, class ConstBuffer>;

class ConstBuffer {
 protected:
  cb_size size_;
  cb_id id_;

 public:
  cb_id id() const;
  cb_size size() const;
  ConstBuffer();
  ConstBuffer(cb_id id, cb_size s);
};

template <typename Init>
D3D11_SUBRESOURCE_DATA data(const Init& data);
D3D11_BUFFER_DESC desc(int byte_width, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access);
