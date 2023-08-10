#pragma once
//#include "AbstractFigure.h"
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
struct AdvVertex {
	XMFLOAT3 pos;
	XMFLOAT4 color;
};

class Pyramid 
{
public:
	Pyramid(ID3D11Device* i_d3dDevice, ID3D11DeviceContext* i_context, D3D_PRIMITIVE_TOPOLOGY drawMode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	void render(ID3D11DeviceContext* i_context);
	void bindVShader(ID3D11VertexShader* v) { vShader = v; }
	void bindPShader(ID3D11PixelShader* p) { pShader = p; }
	void bindShaders(ID3D11VertexShader* v, ID3D11PixelShader* p) {  vShader = v; pShader = p; }
	ID3D11Buffer* getConstBuffer() { return constBuffer; }
	void move(float x, float y, float z);
protected:  		 
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer; 
	ID3D11Buffer* constBuffer;
	ID3D11VertexShader* vShader;
	ID3D11PixelShader* pShader;
	
	void setBufferData(D3D11_SUBRESOURCE_DATA* bdata, void* w);
	void setBuffer(D3D11_BUFFER_DESC* bd, int byteWidth, D3D11_BIND_FLAG bindBuffer, D3D11_USAGE usage = D3D11_USAGE_DEFAULT);
};

