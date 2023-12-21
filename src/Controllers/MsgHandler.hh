#pragma once
#include <Rendering/Window.h>
#include <_Commons/vertex.h>
#include <_Commons/alias.h>


enum Keys
{
  Key_escape = VK_ESCAPE,
  Key_f11 = VK_F11
};

class MsgHandler {
 private:
  SHORT keys[256];
  bool catched_quit;
  Keys keyboard_buttons;
  DWORD last_ticks;


 public:
  static stlcwstr last_entity_name;
  MSG message_;
  POINT cursor;
  POINT middle_cursor;
  MsgHandler();
  ~MsgHandler();
  bool catched_message();

  bool is_pressed(char keychar);
  int get_cursor_dx();
  int get_cursor_dy();
  bool move_event(char keychar);
  bool is_quit();
  
  void show_cursor();
  void hide_cursor();
  void set_cursor_middle();
  

  DWORD cur_frame_ticks();
  DWORD total_ticks();
  bool tick();

  UINT wParam() const {
    return message_.wParam;
  }
  UINT message() const {
    return message_.message;
  }
};
