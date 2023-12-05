#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <windows.h>

class Device {
 private:
  D3D_DRIVER_TYPE driverType;
  D3D_FEATURE_LEVEL featureLevel;
  IDXGISwapChain* swapChain;

  ID3D11RenderTargetView* renderTargetView;
  bool fullscreen;

  DXGI_SWAP_CHAIN_DESC setSwapChainDesc(
      HWND descriptor,
      int bufcount,
      DXGI_USAGE usage,
      int width,
      int height,
      bool isWindowed = false,
      int refreshRate = 60,
      int refreshRateDenom = 1,
      DXGI_FORMAT pixelFormat = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
  D3D11_VIEWPORT
  setViewPort(float width, float height, float minDepth = 0.f, float maxDepth = 1.f, float topleftX = 0.f, float topleftY = 0.f);

 public:
  static ID3D11Device* d3d;
  static ID3D11DeviceContext* ic;
  Device(HWND wd);
  ~Device();
  

  void renderStart();
  void renderEnd();

  inline IDXGISwapChain* getSwapChain() {
    return swapChain;
  }
  inline ID3D11RenderTargetView* getTargetView() {
    return renderTargetView;
  }
  void switchFullScreen();
  void release();
};
