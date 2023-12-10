#include "Window.h"

#define H_ERRMSG(hr, text) if (FAILED(hr)){ MessageBox( NULL, text, L"Ошибка окна", MB_OK | MB_ICONHAND); exit(-1);}
#define H_WARNMSG(hr, text) if (FAILED(hr)) MessageBox( NULL, text, L"Ошибка окна", MB_OK | MB_ICONEXCLAMATION);

int Window::width_;
int Window::height_;
HWND Window::descriptor;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	switch (wParam) {
	case VK_ESCAPE:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

Window::Window(HINSTANCE hInstance, int w, int h) { 
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
    wcex.hIconSm = LoadIcon(hInstance, (LPCWSTR)IDI_ICON1);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"Новый холст";

	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, w, h };	
	width_ = rc.right - rc.left;
	height_ = rc.bottom - rc.top;
	AdjustWindowRect(&rc, WS_POPUPWINDOW, FALSE);
	descriptor = CreateWindow(L"Новый холст", L"ПАТРИК ИГРА!", WS_POPUPWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width_, height_, nullptr, nullptr, hInstance, nullptr);
	ShowWindow(descriptor, 10);

}

Window::~Window() 
{
  release();
}

void Window::release() {
	DestroyWindow(descriptor);
}

