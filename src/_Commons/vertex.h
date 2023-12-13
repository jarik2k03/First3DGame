#pragma once
#ifndef VERTEX_H
#define VERTEX_H
#include <xnamath.h>
#include <debugapi.h>
#include <sstream>

#define CONSOLEDEBUG(ss) OutputDebugStringA(ss.str().c_str());



struct ConstantBuffer {
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

struct AdvVertex {
  XMFLOAT3 pos;
  XMFLOAT2 texture;
};

struct position {
  float x, y, z;
};

static void debug_matrixes(XMMATRIX& world, XMMATRIX& view, XMMATRIX& proj) {
  std::stringstream ss;
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