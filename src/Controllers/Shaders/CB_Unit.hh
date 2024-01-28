#pragma once
#include <Controllers/Shaders/ConstBuffer.hh>
class CB_Unit : public ConstBuffer {
 private:
  mutable ID3D11Buffer* buf;

 public:
  CB_Unit();
  ~CB_Unit();
  CB_Unit(const ConstBuffer& copy, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access);
  template <typename Init>
  CB_Unit(const ConstBuffer& copy, const Init& init_data, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access);
  ID3D11Buffer*& operator*() const;
  CB_Unit& operator=(CB_Unit& newbie);
  cb_id id() const;
};

CB_Unit make_cb_unit(const ConstBuffer& copy, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access);