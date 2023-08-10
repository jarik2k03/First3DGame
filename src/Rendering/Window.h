#pragma once
#include <d3dx11.h>
#include <windows.h>
#include <iostream>
#include "../Resources/resource.h"

class Window
{
private:
	HWND descriptor;
	WNDCLASSEX wcex;
	int width;
	int height;
public:
	inline HWND getWd() { return this->descriptor;}
	Window(HINSTANCE hInstance, int w = 1920, int h = 1080);
	~Window();

};

void showFPS(HDC hdc);