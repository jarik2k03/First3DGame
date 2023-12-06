#include "MsgHandler.h"

MsgHandler::MsgHandler() {
  last_ticks = GetTickCount64();
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
  return message_.wParam != key ? false : true;
}

bool MsgHandler::is_pressed(char keychar) {
  return message_.wParam != keychar ? false : true;
}

bool MsgHandler::move_event(char keychar) {
  return message_.wParam == keychar && message_.message == WM_KEYDOWN ? true : false;
}

bool MsgHandler::tick() {
  sstream ss;
  ss << "TICKS: " << GetTickCount64() - last_ticks << "\n";
  CONSOLEDEBUG(ss);
  DWORD current = GetTickCount64() - last_ticks;
  if (current > 12) {
    last_ticks = GetTickCount64();
    return true;
  }
  return false;
}