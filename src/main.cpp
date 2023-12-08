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
  Device directx(window.getWd());
  MsgHandler messenger;
  Shaders shaderController;
  World overlord;
  Camera eye(overlord.m_world(), XM_PIDIV4 / 1.5);
  ID3D11VertexShader* v = shaderController.addVertexShader(SHADERPATH, "VS_Out");
  ID3D11PixelShader* p = shaderController.addPixelShader(SHADERPATH, "PS_Out");
  Pyramid lipton(10, -1.5, 0, v, p);
  Cube cubik(0, -1.5, 0, v, p);
  
  

  while (messenger.message() != WM_QUIT) {
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

      if (messenger.is_pressed('w')) {
        eye.move(0.0f, 0.0f, 0.22f);
      }
      if (messenger.is_pressed('a')) {
        eye.move(-0.22f, 0.0f, 0.0f);
      }
      if (messenger.is_pressed('s')) {
        eye.move(0.0f, 0.0f, -0.22f);
      }
      if (messenger.is_pressed('d')) {
        eye.move(0.22f, 0.0f, 0.0f);
      }
      if (messenger.is_pressed('+')) {
        messenger.show_cursor(true);
      }
      if (messenger.is_pressed('-')) {
        messenger.show_cursor(false);
      }

      eye.rotate_x(messenger.get_cursor_dx());
      eye.rotate_y(messenger.get_cursor_dy());
      messenger.set_cursor_middle();

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
