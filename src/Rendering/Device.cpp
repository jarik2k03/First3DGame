#include "Device.h"

#define H_WARNMSG(hr, text) \
  if (FAILED(hr))           \
    MessageBox(NULL, text, L"Ошибка драйвера", MB_OK | MB_ICONEXCLAMATION);

// статические интерфейсы
ID3D11Device* Device::d3d;
ID3D11DeviceContext* Device::ic;
ID3D11Buffer* Device::const_buf_;

Device::Device() : featureLevel(D3D_FEATURE_LEVEL_11_0), driverType(D3D_DRIVER_TYPE_NULL), swapChain(NULL), renderTargetView(NULL) {
  RECT rect;
  GetClientRect(Window::descriptor, &rect);
  int width = rect.right - rect.left; //+16;
  int height = rect.bottom - rect.top; //+39;

  D3D_DRIVER_TYPE driverTypes[] = {
      D3D_DRIVER_TYPE_HARDWARE,
      D3D_DRIVER_TYPE_WARP,
      D3D_DRIVER_TYPE_REFERENCE,
  };
  D3D_FEATURE_LEVEL featureLevels[] = {
      D3D_FEATURE_LEVEL_11_0,
      D3D_FEATURE_LEVEL_10_1,
      D3D_FEATURE_LEVEL_10_0,
  };
  // цепь связи
  DXGI_SWAP_CHAIN_DESC sc =
      setSwapChainDesc(Window::descriptor, 1, DXGI_USAGE_RENDER_TARGET_OUTPUT, width, height, true, 60, 1, DXGI_FORMAT_R8G8B8A8_UNORM);
  HRESULT state = S_OK;
  for (int driverIndex = 0; driverIndex < ARRAYSIZE(driverTypes); driverIndex++) {
    driverType = driverTypes[driverIndex];
    state = D3D11CreateDeviceAndSwapChain(
        NULL, driverType, NULL, 0, featureLevels, ARRAYSIZE(featureLevels), D3D11_SDK_VERSION, &sc, &swapChain, &d3d, &featureLevel, &ic);
    if (SUCCEEDED(state))
      break;
  }
  H_WARNMSG(state, L"Не найдено подходящей версии драйвера Direct3d");

  /* TargetView */
  ID3D11Texture2D* backBuffer;
  state = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
  H_WARNMSG(state, L"Не удалось получить буфер");
  // RenderTargetOutput - передний буфер, RenderTargetView - задний
  state = d3d->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView);
  H_WARNMSG(state, L"Не удалось задать задний буфер");
  backBuffer->Release();
  /* STENCIL */
  D3D11_TEXTURE2D_DESC depth = setDescDepth(width, height);
  state = d3d->CreateTexture2D(&depth, NULL, &depth_stencil);
  H_WARNMSG(state, L"Не удалось задать текстуру глубин");

  D3D11_DEPTH_STENCIL_VIEW_DESC dsv = setDSV();
  state = d3d->CreateDepthStencilView(depth_stencil, &dsv, &depth_stencil_view);
  H_WARNMSG(state, L"Не создался буфер глубин");
  ic->OMSetRenderTargets(1, &renderTargetView, depth_stencil_view);
  /* ViewPort*/
  D3D11_VIEWPORT vp = setViewPort(width, height);
  ic->RSSetViewports(1, &vp); // привязываем к контексту устройства
}

void Device::release() {
  if (depth_stencil)
    depth_stencil->Release();
  if (depth_stencil_view)
    depth_stencil_view->Release();
  if (ic)
    ic->ClearState();
  if (renderTargetView)
    renderTargetView->Release();
  if (swapChain)
    swapChain->Release();
  if (ic)
    ic->Release();
  if (d3d)
    d3d->Release();
}

Device::~Device() {
  release();
}

DXGI_SWAP_CHAIN_DESC Device::setSwapChainDesc(
    HWND descriptor,
    int bufcount,
    DXGI_USAGE usage,
    int width,
    int height,
    bool isWindowed,
    int refreshRate,
    int refreshRateDenom,
    DXGI_FORMAT pixelFormat) {
  DXGI_SWAP_CHAIN_DESC sc;
  ZeroMemory(&sc, sizeof(sc));
  sc.BufferCount = bufcount; // один задний буфер
  sc.BufferDesc.Width = width;
  sc.BufferDesc.Height = height;
  sc.BufferDesc.Format = pixelFormat;
  sc.BufferDesc.RefreshRate.Numerator = refreshRate; // частота обн экрана
  sc.BufferDesc.RefreshRate.Denominator = refreshRateDenom;
  sc.BufferUsage = usage; // назначение - задний буфер
  sc.OutputWindow = descriptor;
  sc.SampleDesc.Count = 1;
  sc.SampleDesc.Quality = 0;
  sc.Windowed = isWindowed;
  fullscreen = isWindowed; // сохранение состояния
  return sc;
}

D3D11_TEXTURE2D_DESC Device::setDescDepth(int width, int height, int interpolLev, D3D11_USAGE usage, DXGI_FORMAT pixelFormat) {
  D3D11_TEXTURE2D_DESC depth;
  ZeroMemory(&depth, sizeof(depth));
  depth.Width = width;
  depth.Height = height;
  depth.MipLevels = interpolLev;
  depth.ArraySize = 1;
  depth.Format = pixelFormat;
  depth.SampleDesc.Count = 1;
  depth.SampleDesc.Quality = 0;
  depth.Usage = usage;
  depth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depth.CPUAccessFlags = 0;
  depth.MiscFlags = 0;
  return depth;

}

D3D11_DEPTH_STENCIL_VIEW_DESC Device::setDSV(D3D11_DSV_DIMENSION dimension, DXGI_FORMAT pixelFormat) {
  D3D11_DEPTH_STENCIL_VIEW_DESC dsv;
  ZeroMemory(&dsv, sizeof(dsv));
  dsv.ViewDimension = dimension;
  dsv.Format = pixelFormat;
  dsv.Texture2D.MipSlice = 0;
  return dsv;
}

D3D11_VIEWPORT Device::setViewPort(float w, float h, float minDepth, float maxDepth, float topleftX, float topleftY) {
  D3D11_VIEWPORT vp;
  vp.Width = w;
  vp.Height = h;
  vp.MinDepth = minDepth;
  vp.MaxDepth = maxDepth;
  vp.TopLeftX = topleftX;
  vp.TopLeftY = topleftY;
  return vp;
}

void Device::render_start() {
  // очищаем задний буфер
  float ClearColor[4] = {0.002, 0.003, 0.001, 0.159};
  ic->ClearRenderTargetView(renderTargetView, ClearColor);
  ic->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH , 1.0f, 0);
  // выброс буфера на экран
}

void Device::render_end() {
  swapChain->Present(1, 0);
}

void Device::switchFullScreen() {
  fullscreen = (bool)fullscreen ^ 1;
  swapChain->SetFullscreenState(fullscreen, NULL);
  
}
