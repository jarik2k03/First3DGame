#include <Controllers/MsgHandler.h>
#include <Entity/Camera.h>
#include <Figures/Cube.h>
#include <Figures/Pyramid.h>
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
  Camera eye(XM_PIDIV4 / 1.5, {-10, 0, -20});
  ID3D11VertexShader* v = shaderController.addVertexShader(SHADERPATH, "VS_Out");
  ID3D11PixelShader* p = shaderController.addPixelShader(SHADERPATH, "PS_Out");
  Cube cubik({10, -1.5, 25}, v, p);
  Cube cubb({10, -4.5, 35}, v, p);

  while (!messenger.is_quit()) {
    if (messenger.catched_message()) {
      if (messenger.is_pressed(VK_ESCAPE)) {
        exit(0);
      }
      if (messenger.is_pressed(VK_F11)) {
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
        eye.move_straight(0.4f);
      }
      if (messenger.is_pressed('S')) {
        eye.move_straight(-0.2f);
      }
      if (messenger.is_pressed('A')) {
        eye.move_side(-0.3f);
      }
      if (messenger.is_pressed('D')) {
        eye.move_side(0.3f);
      }
      if (messenger.is_pressed(VK_SPACE)) {
        eye.move({0.0f, 0.3f, 0.0f});
      }
      if (messenger.is_pressed(VK_SHIFT)) {
        eye.move({0.0f, -0.3f, 0.0f});
      }

      eye.rotate(messenger.get_cursor_dx(), messenger.get_cursor_dy());

    matrixes:
      cubik.update_state(eye.m_view(), eye.m_proj());
      cubb.update_state(eye.m_view(), eye.m_proj());

    render:
      directx.renderStart();
      cubik.render();
      cubb.render();
      directx.renderEnd();
    }
  }
  return 0;
}
