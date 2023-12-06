#pragma once
#ifndef ALIAS_H
#define ALIAS_H


#include <string>
#include <sstream>
#include <debugapi.h>
#include <xnamath.h>

#define CONSOLEDEBUG(ss) \
    OutputDebugStringA(ss.str().c_str());



struct ConstantBuffer {
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

using stlstr = std::string;
using stlcstr = const std::string;
using stlwstr = std::wstring;
using stlcwstr = const std::wstring;
using sstream = std::stringstream;

static void debug_matrixes(XMMATRIX& world ,XMMATRIX& view, XMMATRIX& proj) {
  sstream ss;
  ss << "WORLD:\n";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ss << world(i, j) << " ";
    }
    ss << "\n";
  }
  ss << "VIEW\n";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ss << view(i, j) << " ";
    }
    ss << "\n";
  }
  ss << "PROJECTION\n";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      ss << proj(i, j) << " ";
    }
    ss << "\n";
  }
  ss << "\n";

  CONSOLEDEBUG(ss);
}

#endif