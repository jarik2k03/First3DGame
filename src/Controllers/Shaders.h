#pragma once
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <d3dcompiler.h>
#include <map>

class Shaders {
 public:
  Shaders();
  ~Shaders();
  ID3D11VertexShader* getVertexShader(stlstr& entryPoint);
  ID3D11InputLayout* getVertexLayout(stlstr& entryPoint);
  ID3D11PixelShader* getPixelShader(stlstr& entryPoint);
  ID3D11VertexShader* addVertexShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel = "vs_5_0");
  ID3D11PixelShader* addPixelShader(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel = "ps_5_0");

 protected:
  std::map<stlstr, ID3D11VertexShader*> vertexShaders; // набор вершинных шейдеров
  std::map<stlstr, ID3D11InputLayout*> vertexLayouts; // набор вершинных макетов
  std::map<stlstr, ID3D11PixelShader*> pixelShaders; // набор пиксельных шейдеров

  HRESULT compileFromFile(stlcwstr& filename, stlcstr& entryPoint, stlcstr& shaderModel, ID3DBlob** blobOut);
};
