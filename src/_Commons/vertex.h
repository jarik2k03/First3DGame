#pragma once
#ifndef VERTEX_H
#define VERTEX_H
#include <debugapi.h>
#include <xnamath.h>
#include <sstream>

#define COUT(ss) OutputDebugStringA(ss.str().c_str());
#define COUTNL(ss) \
  ss << '\n';      \
  OutputDebugStringA(ss.str().c_str());

#define X0 0.0f
#define X1 1.0f
#define Z0 0.0f
#define Z1 1.0f

constexpr int chunk_size() {
  return 16 * 16 * 16;
}

using PositionBuffer = XMMATRIX;
using ViewBuffer = XMMATRIX;
using ProjBuffer = XMMATRIX;
struct BlockBuffer {
  uint32_t* blocks;
  int counter;
  BlockBuffer(uint32_t* data, int cnt) : blocks(data), counter(cnt) {
  }
};

struct CounterBuf {
  int cnt;
  int size;
  int area;
  CounterBuf(int c, int sz, int a) : cnt(c), size(sz), area(a) {
  }
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

#endif