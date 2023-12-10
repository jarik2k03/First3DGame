#include <Controllers/MsgHandler.h>
#include <Entity/Camera.h>
#include <Figures/Pyramid.h>
#include <Figures/Cube.h>
#include <Figures/Triangle.h>
#include <Rendering/Device.h>
#include <Rendering/Shaders.h>
#include <Rendering/Window.h>
#include <World.h>

#define SHADERPATH L"src\\Shaders\\simple.fx"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  Window window(hInstance, 1280, 720);
  Device directx;
  MsgHandler messenger;
  Shaders shaderController;
  World overlord;
  Camera eye(overlord.m_world(), XM_PIDIV4 / 1.5);
  ID3D11VertexShader* v = shaderController.addVertexShader(SHADERPATH, "VS_Out");
  ID3D11PixelShader* p = shaderController.addPixelShader(SHADERPATH, "PS_Out");
  Cube cubik(0, -1.5, 0, v, p);
  Pyramid lipton(10, -1.5, 15, v, p);

  while (messenger.is_quit()) {
    if (messenger.catched_message()) {
      if (messenger.is_pressed(Key_escape)) {
        exit(0);
      }
      if (messenger.is_pressed(Key_f11)) {
        directx.switchFullScreen();
      }
    } else {
      if (!messenger.tick())
        goto render;

      if (messenger.is_pressed(VK_CONTROL)) {
        messenger.show_cursor();  
        goto matrixes;
      }
      
      messenger.hide_cursor();  
      messenger.set_cursor_middle();
      if (messenger.is_pressed('W')) {
        eye.move(0.0f, 0.0f, 1.0f);
      }
      if (messenger.is_pressed('A')) {
        eye.move(-1.0f, 0.0f, 0.0f);
      }
      if (messenger.is_pressed('S')) {
        eye.move(0.0f, 0.0f, -1.0f);
      }
      if (messenger.is_pressed('D')) {
        eye.move(1.0f, 0.0f, 0.0f);
      }
      eye.rotate(messenger.get_cursor_dx() * 10, messenger.get_cursor_dy() * 10);

    matrixes:
      eye.fix_position(overlord.m_world(), lipton.constBuffer());

    render:
      directx.renderStart();
      cubik.render();
      lipton.render();
      directx.renderEnd();
    }
  }
  return 0;
}
