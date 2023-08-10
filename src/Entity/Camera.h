#pragma once
#include <windows.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>

class Camera
{
public:
	Camera(HWND hWnd, FLOAT objectiveWidth);
	XMMATRIX* getViewMatrix() { return &matrixView; }
	XMMATRIX* getProjMatrix() { return &matrixPerspective; }
protected:
	XMMATRIX matrixView;
	XMMATRIX matrixPerspective;
};

