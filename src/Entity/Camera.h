#pragma once
#include <Rendering/Device.h>
#include <xnamath.h>
#include<_Commons/alias.h>

class Camera
{
public:
	Camera(int w, int h, FLOAT objectiveWidth);
	XMMATRIX& m_view() { return m_view_; }
	XMMATRIX& m_proj() { return m_proj_; }
protected:
 XMMATRIX m_view_;
 XMMATRIX m_proj_;
};

