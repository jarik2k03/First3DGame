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

constexpr int chunk_size() {
  return 16 * 16 * 16;
}

struct ConstantBuffer {
  XMMATRIX world;
};

struct CamBuffer {
  XMMATRIX view;
  XMMATRIX proj;
};

struct LightBuffer {
  XMFLOAT4 dir;
  XMFLOAT4 src_color;
  XMFLOAT4 dest_color;
};

struct AdvVertex {
  XMFLOAT3 pos;
  XMFLOAT2 texture;
  XMFLOAT3 normal;
};

static XMMATRIX m_world;
static ConstantBuffer cb;

#endif