#include "Device.h"
#include <stdio.h>


Device::Device(HWND wd) // wd - ���������� ����
{	

	RECT rect;
	GetClientRect(wd, &rect); 
	int width = rect.right - rect.left; //+16;
	int height = rect.bottom - rect.top; //+39;
	featureLevel = D3D_FEATURE_LEVEL_11_0;
	driverType = D3D_DRIVER_TYPE_NULL;
	d3dDevice = NULL;
	swapChain = NULL;
	immediateContext = NULL;
	renderTargetView = NULL;

	setSwapChain(wd, 1, DXGI_USAGE_RENDER_TARGET_OUTPUT, width, height, true, 60, 1, DXGI_FORMAT_R8G8B8A8_UNORM);
	
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	int numDriverTypes = ARRAYSIZE(driverTypes);
	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	int numFeatureLevels = ARRAYSIZE(featureLevels);
	// ���� �����
	
	HRESULT state = S_OK;
	for (int driverIndex = 0; driverIndex < numDriverTypes; driverIndex++)
	{
		driverType = driverTypes[driverIndex];
		state = D3D11CreateDeviceAndSwapChain(NULL, driverType, NULL, 0,
			featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sc, &swapChain, 
			&d3dDevice, &featureLevel, &immediateContext);
		if (SUCCEEDED(state)) 
			break;
	}
	if (FAILED(state)) errMessage(L"�� ������� ���������� ������ �������� Direct3d");
	ID3D11Texture2D* backBuffer;
	state = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (FAILED(state)) errMessage(L"�� ������� �������� �����");
	// RenderTargetOutput - �������� �����, RenderTargetView - ������ 
	state = d3dDevice->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
	if (FAILED(state)) errMessage(L"�� ������� ������ ������ �����");
	backBuffer->Release();
	// ���������� ������ ����� � ��������� ����������
	immediateContext->OMSetRenderTargets(1, &renderTargetView, nullptr);
	setViewPort(width, height);
	immediateContext->RSSetViewports(1, &vp); // ����������� � ��������� ����������
}


Device::~Device()
{
	if (immediateContext) immediateContext->ClearState();
	if (renderTargetView) renderTargetView->Release();
	if (swapChain) swapChain->Release();
	if (immediateContext) immediateContext->Release();
	if (d3dDevice) d3dDevice->Release();
}

void Device::setSwapChain(HWND descriptor, int bufcount, DXGI_USAGE usage, int width, int height, bool isWindowed, int refreshRate, int refreshRateDenom, DXGI_FORMAT pixelFormat)
{
	ZeroMemory(&sc, sizeof(sc));
	sc.BufferCount = bufcount; // ���� ������ �����
	sc.BufferDesc.Width = width;
	sc.BufferDesc.Height = height;
	sc.BufferDesc.Format = pixelFormat;
	sc.BufferDesc.RefreshRate.Numerator = refreshRate; // ������� ��� ������
	sc.BufferDesc.RefreshRate.Denominator = refreshRateDenom;
	sc.BufferUsage = usage; // ���������� - ������ ����� 
	sc.OutputWindow = descriptor;
	sc.SampleDesc.Count = 1;
	sc.SampleDesc.Quality = 0;
	sc.Windowed = isWindowed;
	fullscreen = isWindowed; // ���������� ���������

}

void Device::setViewPort(float width, float height, float minDepth, float maxDepth, float topleftX, float topleftY)
{
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = minDepth;
	vp.MaxDepth = maxDepth;
	vp.TopLeftX = topleftX;
	vp.TopLeftY = topleftY;
}


void Device::renderStart()
{
	// ������� ������ �����
	float ClearColor[4] = { 0.15f, 0.15f, 0.2f, 1.0f };
	immediateContext->ClearRenderTargetView(renderTargetView, ClearColor);
	// ������ ������ �� �����
	

}

void Device::renderEnd()
{
	swapChain->Present(2, 0);
}


void Device::switchFullScreen()
{
	fullscreen = (bool) fullscreen ^ 1;
	swapChain->SetFullscreenState((~fullscreen) & 0x1, NULL);
}

int Device::errMessage(LPCWSTR text)
{
	MessageBox(NULL, text, L"������ ��������", MB_ICONEXCLAMATION | MB_OK);
	exit(-1);
}

int Device::warnMessage(LPCWSTR text)
{
	MessageBox(NULL, text, L"������ ��������", MB_ICONEXCLAMATION | MB_OK);
	return -1;
}

