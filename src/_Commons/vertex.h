#pragma once
#ifndef VERTEX_H
#define VERTEX_H
#include <xnamath.h>
#include <debugapi.h>
#include <sstream>

#define CONSOLEDEBUG(ss) OutputDebugStringA(ss.str().c_str());

#define X0 0.0f
#define X1 1.0f
#define Z0 0.0f
#define Z1 1.0f

struct ConstantBuffer {
  XMMATRIX world;
  XMMATRIX view;
  XMMATRIX proj;
};

struct AdvVertex {
  XMFLOAT3 pos;
  XMFLOAT2 texture;
};


#endif