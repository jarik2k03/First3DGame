#include <Entity/Camera.h>
#include <Entity/Pyramid.h>
#include <Entity/Triangle.h>
#include <Rendering/Device.h>
#include <Rendering/Shaders.h>
#include <Rendering/Window.h>
#include <World.h>

#define SHADERPATH L"src\\Shaders\\simple.fx"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  Window window(hInstance, 1280, 720);
  Device directx(window.getWd());
  Shaders shaderController;
  World overlord;
  Camera eye(window.width(), window.height(), XM_PIDIV4);
  ID3D11VertexShader* v = shaderController.addVertexShader(SHADERPATH, "VS_Out");
  ID3D11PixelShader* p = shaderController.addPixelShader(SHADERPATH, "PS_Out");
  Pyramid piramidka(v, p);
  
  MSG msg = {0};
  while (msg.message != WM_QUIT && msg.wParam != VK_ESCAPE) {
    if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);

      if (msg.wParam == VK_F11)
        directx.switchFullScreen();
    } else {
      overlord.UpdateMatrixes(eye.m_view(), eye.m_proj(), piramidka.constBuffer());
      directx.renderStart();
      piramidka.render();
      directx.renderEnd();
    }
  }
  return 0;
}
