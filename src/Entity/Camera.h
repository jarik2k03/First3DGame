#pragma once
#include <Rendering/Device.h>
#include <xnamath.h>
#include<_Commons/alias.h>
#include <unordered_map>


using entity_buflist = std::unordered_map<stlstr, ID3D11Buffer*>;

class Camera
{
public:
	Camera(int w, int h, FLOAT objectiveWidth);
	void fix_position(XMMATRIX& in_world, entity_buflist& figures);
    void move(const float dx, const float dy, const float dz);
        void rotate(float angle);
	XMMATRIX& m_view() { return m_view_; }
	XMMATRIX& m_proj() { return m_proj_; }
protected:
 
 XMMATRIX m_view_;
 XMMATRIX m_proj_;
 float fov;
 float x, y, z;
 float rotation_angle;
};

