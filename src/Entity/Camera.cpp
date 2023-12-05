#include "Camera.h"

Camera::Camera(int w, int h, FLOAT objectiveWidth) {
  XMVECTOR eyePos = XMVectorSet(0.0f, 1.0f, -5.0f, 0.0f);
  XMVECTOR eyeDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  XMVECTOR upDir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
  m_view_ = XMMatrixLookAtLH(eyePos, eyeDir, upDir); // LH наша координатная система
  m_proj_ = XMMatrixPerspectiveFovLH(objectiveWidth, w / (float)h, 0.01f, 100.0f);

  // 3 аргумент - самое ближнее видимое расстояние, 4 аргумент - самое дальнее
}
