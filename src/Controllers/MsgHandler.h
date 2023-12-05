#pragma once
#include <windows.h>

enum Keys
{
  Key_escape = VK_ESCAPE,
  Key_f11 = VK_F11
};

class MsgHandler {
 private:
  MSG message_, prev_message_;
  Keys keyboard_buttons;

 public:
  MsgHandler();
  bool catched_message();
  bool is_pressed(Keys key);

  UINT wParam() const {
    return message_.wParam;
  }
  UINT message() const {
    return message_.message;
  }
};
