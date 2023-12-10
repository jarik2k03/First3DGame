#pragma once

#include <windows.h>
#include <Resources/resource.h>

class Window {
 private:
  


 public:
  static HWND descriptor;
  static int width_;
  static int height_;
  HWND getWd() {
    return descriptor;
  }
  Window(HINSTANCE hInstance, int w = 1920, int h = 1080);
  ~Window();
  void release();
};
