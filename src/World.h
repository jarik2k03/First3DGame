#pragma once
#include <Rendering/Device.h>
#include <_Commons/alias.h>
#include <xnamath.h>
#include <stdio.h>



class World
{
public:
	World();
	XMMATRIX& m_world();
protected:
	XMMATRIX m_world_;

	void debug_matrixes(XMMATRIX& view, XMMATRIX& proj);
};

