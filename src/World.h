#pragma once
#include <Rendering/Device.h>
#include <_Commons/alias.h>
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
	XMMATRIX& m_world();
	void UpdateMatrixes(XMMATRIX& view, XMMATRIX& proj, ID3D11Buffer* cBuffer);
protected:
	XMMATRIX m_world_;

	void debug_matrixes(XMMATRIX& view, XMMATRIX& proj);
};

