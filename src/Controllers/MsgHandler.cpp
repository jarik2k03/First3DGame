#include "MsgHandler.hh"

stlcwstr MsgHandler::last_entity_name;

MsgHandler::MsgHandler() : catched_quit(false) {
  last_ticks = GetTickCount64();
  ZeroMemory(&message_, sizeof(message_));
  middle_cursor = {Window::width_ >> 1, Window::height_ >> 1};
}

MsgHandler::~MsgHandler() {
}

bool MsgHandler::catched_message() {
  while (PeekMessage(&message_, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&message_);
    DispatchMessage(&message_);
    if (message_.message == WM_QUIT) {
      catched_quit = true;
      return true;
    }
    for (int i = 0; i < 256; i++) {
      keys[i] = GetAsyncKeyState(i);
    }
    GetCursorPos(&cursor);
    return true;
  }
  return false;
}

bool MsgHandler::is_quit() {
  return catched_quit;
}

bool MsgHandler::is_pressed(char keychar) {
  return keys[(int)keychar];
}

void MsgHandler::show_cursor() {
  ShowCursor(true);
}

void MsgHandler::hide_cursor() {
  ShowCursor(false);
}

int MsgHandler::get_cursor_dx() {
  return cursor.x - middle_cursor.x;
}

int MsgHandler::get_cursor_dy() {
  return cursor.y - middle_cursor.y;
}

void MsgHandler::set_cursor_middle() {
  SetCursorPos(middle_cursor.x, middle_cursor.y);
}

bool MsgHandler::move_event(char keychar) {
  return message_.wParam == keychar && message_.message == WM_KEYDOWN ? true : false;
}

DWORD MsgHandler::cur_frame_ticks() {
  return GetTickCount64() - last_ticks;
}

DWORD MsgHandler::total_ticks() {
  return GetTickCount64();
}

bool MsgHandler::tick() {
  sstream ss;
  ss << "TICKS: " << GetTickCount64() - last_ticks << "\n";
  // CONSOLEDEBUG(ss);
  DWORD current = GetTickCount64() - last_ticks;
  if (current > 12) {
    last_ticks = GetTickCount64();
    return true;
  }
  return false;
}