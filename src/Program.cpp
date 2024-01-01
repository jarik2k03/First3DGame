#include <Rendering/Modeler.hh>
#include <Entity/Camera.hh>
#include <Entity/Entity.hh>
#include <Entity/Chunk.hh>

#include <Controllers/MsgHandler.hh>
#include <Controllers/Shaders.h>

#include <Rendering/Window.h>
#include <Rendering/Device.h>

#include <_Commons/model_layouts.h>

#define SHADERPATH L"src\\Shaders\\simple.fx"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  Window window(hInstance, 1280, 720);
  Device directx;
  MsgHandler messenger;
  Shaders shader_controller;
  Modeler model_creator;
  Camera eye(110.0f, {10, 0, 20});
  ID3D11VertexShader* v = shader_controller.addVertexShader(SHADERPATH, "VS_Out");
  ID3D11PixelShader* p = shader_controller.addPixelShader(SHADERPATH, "PS_Out");
  ID3D11PixelShader* plight = shader_controller.addPixelShader(SHADERPATH, "PS_Light");
  model_creator.create_model(L"Cube", cub_verts(), cub_idx());
  //model_creator.create_model(L"Pyramid", pyramid_verts(), pyramid_idx());
  auto cub = model_creator.get_model(L"Cube");


  Chunk tiny(10, {0, 0, 0}, cub, v, p);

  Entity lamp({-7, -100, 3}, model_creator.get_model(L"Cube"), v, plight);
  lamp.set_glowing(true);

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

      if (messenger.is_pressed('W') && !messenger.is_pressed(VK_F3)) {
        eye.move_straight(0.4f);
      }
      if (messenger.is_pressed('S') && !messenger.is_pressed(VK_F3)) {
        eye.move_straight(-0.2f);
      }
      if (messenger.is_pressed('A') && !messenger.is_pressed(VK_F3)) {
        eye.move_side(-0.3f);
      }
      if (messenger.is_pressed('D') && !messenger.is_pressed(VK_F3)) {
        eye.move_side(0.3f);
      }
      if (messenger.is_pressed(VK_SPACE)) {
        eye.move({0.0f, 0.3f, 0.0f});
      }
      if (messenger.is_pressed(VK_SHIFT)) {
        eye.move({0.0f, -0.3f, 0.0f});
      }
      eye.rotate(messenger.get_cursor_dx(), messenger.get_cursor_dy());

      if (messenger.is_pressed('W') && messenger.is_pressed(VK_F3)) {
        lamp.move({0.4f, 0.0f, 0.0f});
      }
      if (messenger.is_pressed('S') && messenger.is_pressed(VK_F3)) {
        lamp.move({-0.4f, 0.0f, 0.0f});
      }
      if (messenger.is_pressed('A') && messenger.is_pressed(VK_F3)) {
        lamp.move({0.0f, 0.0f, -0.3f});
      }
      if (messenger.is_pressed('D') && messenger.is_pressed(VK_F3)) {
        lamp.move({0.0f, 0.0f, 0.3f});
      }

  matrixes:

    render:
      directx.render_start();
      lamp.update_state(eye.m_view(), eye.m_proj());
      eye.update();
      tiny.update_render(eye.m_view(), eye.m_proj());
      
      lamp.render();
      directx.render_end();
    }
  }
  return 0;
}