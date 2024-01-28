#include "ConstBuffer.hh"
#define WARNMSG(text) MessageBoxA(NULL, text, "Конструктор", MB_OK | MB_ICONEXCLAMATION);

ConstBuffer::ConstBuffer(){
}

ConstBuffer::ConstBuffer(cb_id id, cb_size s) : id_(id), size_(s){
}

template <typename Init>
D3D11_SUBRESOURCE_DATA data(const Init& data) {
  return {data, 0, 0};
}

D3D11_BUFFER_DESC desc(int byte_width, D3D11_USAGE usage, D3D11_CPU_ACCESS_FLAG cpu_access) {
  D3D11_BUFFER_DESC bd = {0};
  bd.Usage = usage;
  bd.ByteWidth = byte_width;
  bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // - тип буфера константный буфер
  bd.CPUAccessFlags = cpu_access;
  return bd;
}

cb_id ConstBuffer::id() const {
  return id_;
}
cb_size ConstBuffer::size() const {
  return id_;
}