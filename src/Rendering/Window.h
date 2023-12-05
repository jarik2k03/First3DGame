#pragma once

#include <windows.h>
#include <Resources/resource.h>

class Window {
 private:
  HWND descriptor;
  WNDCLASSEX wcex;
  int width_;
  int height_;

 public:
  int width() {
    return width_;
  }
  int height() {
    return height_;
  }
  HWND getWd() {
    return descriptor;
  }
  Window(HINSTANCE hInstance, int w = 1920, int h = 1080);
  ~Window();
  void release();
};
