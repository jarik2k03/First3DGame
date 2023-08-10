#pragma once
//#include "AbstractFigure.h"
#include <d3d11.h>
#include <d3dx11.h>

struct Vertex {
	float x, y, z;
};

class Triangle 
{
public:
	Triangle(ID3D11Device* i_d3dDevice, ID3D11DeviceContext* i_context, Vertex* i_firstCoords, D3D_PRIMITIVE_TOPOLOGY drawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void render(ID3D11DeviceContext* i_context);
	void bindVShader(ID3D11VertexShader* v) { vShader = v; }
	void bindPShader(ID3D11PixelShader* p) { pShader = p; }
	void bindShaders(ID3D11VertexShader* v, ID3D11PixelShader* p) {  vShader = v; pShader = p; }
	void move(float x, float y, float z);
protected:  		 
	ID3D11Buffer* vertexBuffer;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	Vertex vertices[3];
	
	void setBufferData(D3D11_SUBRESOURCE_DATA* bdata);
	void setBuffer(D3D11_BUFFER_DESC* bd, D3D11_USAGE usage = D3D11_USAGE_DEFAULT, D3D11_BIND_FLAG bindBuffer = D3D11_BIND_VERTEX_BUFFER);
};

