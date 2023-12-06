#pragma once
#include <windows.h>
#include <_Commons/alias.h>
enum Keys
{
  Key_escape = VK_ESCAPE,
  Key_f11 = VK_F11
};

class MsgHandler {
 private:
  MSG message_, prev_message_;
  Keys keyboard_buttons;
  DWORD last_ticks;
 public:
  MsgHandler();
  bool catched_message();
  bool is_pressed(Keys key);
  bool is_pressed(char keychar);
  bool move_event(char keychar);

  bool tick();

  UINT wParam() const {
    return message_.wParam;
  }
  UINT message() const {
    return message_.message;
  }
};
