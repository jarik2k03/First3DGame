#pragma once
#include <d3d11.h>
#include <d3dx11.h>
#include <Rendering/Window.h>

class Device {
 private:
  D3D_DRIVER_TYPE driverType;
  D3D_FEATURE_LEVEL featureLevel;

  IDXGISwapChain* swapChain;
  ID3D11RenderTargetView* renderTargetView;

  ID3D11Texture2D* depth_stencil;
  ID3D11DepthStencilView* depth_stencil_view;
  
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
  D3D11_TEXTURE2D_DESC setDescDepth(
      int width,
      int height,
      int interpolLev = 1,
      D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
      DXGI_FORMAT pixelFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
  D3D11_DEPTH_STENCIL_VIEW_DESC setDSV(
      D3D11_DSV_DIMENSION dimension = D3D11_DSV_DIMENSION_TEXTURE2D,
      DXGI_FORMAT pixelFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
  D3D11_VIEWPORT setViewPort(float w, float h, float minDepth = 0.f, float maxDepth = 1.f, float topleftX = 0.f, float topleftY = 0.f);

 public:
  static ID3D11Buffer* const_buf_;
  static ID3D11Device* d3d;
  static ID3D11DeviceContext* ic;

  Device();
  ~Device();

  void render_start();
  void render_end();

  inline IDXGISwapChain* getSwapChain() {
    return swapChain;
  }
  inline ID3D11RenderTargetView* getTargetView() {
    return renderTargetView;
  }
  void switchFullScreen();
  void release();

};
