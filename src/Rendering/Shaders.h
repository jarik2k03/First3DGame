#pragma once
#include <map>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dcompiler.h>

class Shaders
{

public:
	Shaders();
	~Shaders();
	ID3D11VertexShader* getVertexShader(LPCSTR entryPoint);
	ID3D11InputLayout* getVertexLayout(LPCSTR entryPoint);
	ID3D11PixelShader* getPixelShader(LPCSTR entryPoint);
	ID3D11VertexShader* addVertexShader(ID3D11Device* i_d3dDevice, ID3D11DeviceContext* i_context, LPCWSTR filename, LPCSTR entryPoint, LPCSTR shaderModel);
	ID3D11PixelShader* addPixelShader(ID3D11Device* i_d3dDevice, LPCWSTR filename, LPCSTR entryPoint, LPCSTR shaderModel);

protected:
	std::map <LPCSTR, ID3D11VertexShader*> vertexShaders; // набор вершинных шейдеров
	std::map <LPCSTR, ID3D11InputLayout*> vertexLayouts; // набор вершинных макетов
	std::map <LPCSTR, ID3D11PixelShader*> pixelShaders; // набор пиксельных шейдеров
	
	HRESULT compileFromFile(LPCWSTR szFileName, LPCSTR szEntryPoint, LPCSTR szShaderModel, ID3DBlob** blobOut);
};

