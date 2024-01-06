#pragma once
#ifndef VSHADER_HH
#define VSHADER_HH

#include <Controllers/AShader.hh>

class VShader : public AShader{
 public:
  VShader(ID3D11VertexShader* v) : v(v) {
  }
  ~VShader() {
    v->Release();
  }
 private:
  ID3D11VertexShader* v;

};


#endif // !SHADER_HH
