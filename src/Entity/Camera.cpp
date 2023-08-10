#include "Camera.h"


Camera::Camera(HWND hWnd, FLOAT objectiveWidth)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	XMVECTOR eyePosition = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
	XMVECTOR eyeDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR upDirection = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	matrixView = XMMatrixLookToLH(eyePosition, eyeDirection, upDirection);  // LH наша координатная система
	matrixPerspective = XMMatrixPerspectiveFovLH(objectiveWidth, (float)width / (float)height, 0.01f, 100.0f);
	// 3 аргумент - самое ближнее видимое расстояние, 4 аргумент - самое дальнее

}

