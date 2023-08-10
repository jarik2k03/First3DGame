#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <windows.h>

class Device
{
private:
	DXGI_SWAP_CHAIN_DESC sc;
	D3D_DRIVER_TYPE driverType;
	D3D_FEATURE_LEVEL featureLevel;
	IDXGISwapChain* swapChain;
	ID3D11Device* d3dDevice;
	ID3D11DeviceContext* immediateContext;
	ID3D11RenderTargetView* renderTargetView;
	D3D11_VIEWPORT vp;
	bool fullscreen;

public:
	Device(HWND wd);
	~Device();
	void renderStart();
	void renderEnd();
	void setSwapChain(HWND descriptor, int bufcount, DXGI_USAGE usage, int width, int height, bool isWindowed = false, int refreshRate = 60, int refreshRateDenom = 1, DXGI_FORMAT pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	void setViewPort(float width, float height, float minDepth = 0.f, float maxDepth = 1.f, float topleftX = 0.f, float topleftY = 0.f);
	inline IDXGISwapChain* getSwapChain() { return swapChain; }
	inline ID3D11Device* getDevice() { return d3dDevice; }
	inline ID3D11DeviceContext* getContext() { return immediateContext; }
	inline ID3D11RenderTargetView* getTargetView() { return renderTargetView; }
	int errMessage(LPCWSTR text);
	int warnMessage(LPCWSTR text);
	void switchFullScreen();
};

