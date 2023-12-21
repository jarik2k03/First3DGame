#pragma once
#ifndef AENTITY_HH
#define AENTITY_HH
#include <Rendering/Modeler.hh>
#include <Controllers/MsgHandler.hh>

class AEntity {
 public:
  virtual void update_state(XMMATRIX& view, XMMATRIX& proj) = 0;
  virtual void render() = 0;
  ~AEntity() = default;
 protected:
  virtual D3D11_BUFFER_DESC set_const_buf(int byte_width, D3D11_USAGE usage) = 0;
};

#endif