#include <Controllers/MsgHandler.h>
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
  MsgHandler messenger;
  Shaders shaderController;
  World overlord;
  Camera eye(window.width(), window.height(), XM_PIDIV4);
  ID3D11VertexShader* v = shaderController.addVertexShader(SHADERPATH, "VS_Out");
  ID3D11PixelShader* p = shaderController.addPixelShader(SHADERPATH, "PS_Out");

  Pyramid piramidka(v, p);
  entity_buflist cbuffers;
  cbuffers.insert({"pyr", piramidka.constBuffer()});


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
      if (messenger.is_pressed('u')) {
        eye.rotate(-XM_PIDIV4 / 12);
      }
      if (messenger.is_pressed('i')) {
        eye.rotate(XM_PIDIV4 / 12);
      }
      eye.fix_position(overlord.m_world(), cbuffers);

    render:
      directx.renderStart();
      piramidka.render();
      directx.renderEnd();
    }
  }
  return 0;
}
