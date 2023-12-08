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
  HHOOK mouse_hook;
  
  Keys keyboard_buttons;
  DWORD last_ticks;


 public:
  MSG message_;
  POINT cursor;
  POINT middle_cursor;
  MsgHandler();
  ~MsgHandler();
  bool catched_message();
  bool is_pressed(Keys key);
  bool is_pressed(char keychar);
  int get_cursor_dx();
  int get_cursor_dy();
  bool move_event(char keychar);
  void show_cursor(bool value);
  void set_cursor_middle();
  bool tick();

  UINT wParam() const {
    return message_.wParam;
  }
  UINT message() const {
    return message_.message;
  }
};
