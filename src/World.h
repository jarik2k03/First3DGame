#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <stdio.h>

struct ConstantBuffer {
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX proj;
};

class World
{
public:
	World();
	XMMATRIX* getWorldMatrix();
	void UpdateMatrixes(ID3D11DeviceContext* i_context, XMMATRIX* view, XMMATRIX* perspective, ID3D11Buffer* cBuffer);
protected:
	XMMATRIX worldMatrix;
};

