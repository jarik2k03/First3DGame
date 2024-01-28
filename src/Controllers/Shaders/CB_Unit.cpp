#include "CB_Unit.hh"

CB_Unit::CB_Unit() : buf(NULL) {
}

CB_Unit::CB_Unit(const ConstBuffer& copy, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access) {
  id_ = copy.id(), size_ = copy.size();
  const auto bd = desc(size_, usage, cpu_access);
  auto hr = Device::d3d->CreateBuffer(&bd, NULL, &buf);
  if (FAILED(hr))
    throw "Ошибка создания константного буфера";
}

template <typename Init>
CB_Unit::CB_Unit(const ConstBuffer& copy, const Init& init_data, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access) {
  const auto bd = desc(size_, usage, cpu_access);
  const auto bs = data(init_data);
  auto hr = Device::d3d->CreateBuffer(&bd, &bs, &buf);
  if (FAILED(hr))
    throw "Ошибка создания и инициализации константного буфера";
}

CB_Unit::~CB_Unit() {
  if (buf != NULL)
    buf->Release();
}

ID3D11Buffer*& CB_Unit::operator*() const {
  return buf;
}

CB_Unit& CB_Unit::operator=(CB_Unit& newbie) {
  if (&newbie == this)
    return *this;


}

cb_id CB_Unit::id() const {
  return id_;
}

CB_Unit make_cb_unit(const ConstBuffer& copy, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access) {
  return CB_Unit(copy, usage, cpu_access);
}