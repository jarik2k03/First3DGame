#include "MsgHandler.h"

MsgHandler::MsgHandler() {
  ZeroMemory(&message_, sizeof(message_));
}

bool MsgHandler::catched_message() {
  if (PeekMessage(&message_, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message_);
    DispatchMessage(&message_);
    return true;
  }
  return false;
}

bool MsgHandler::is_pressed(Keys key) {
  return message_.wParam == key ? true : false;
}