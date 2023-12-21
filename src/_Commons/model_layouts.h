#pragma once



vertices cub_verts() {
  vertices cubick(24);
  float fix = 2.0f; // UP DIRECTION
  cubick.at(0) = {XMFLOAT3(-2, fix, -2), XMFLOAT2(X0, Z0), XMFLOAT3(0, fix, 0)};
  cubick.at(1) = {XMFLOAT3(2, fix, -2), XMFLOAT2(X1, Z0), XMFLOAT3(0, fix, 0)};
  cubick.at(2) = {XMFLOAT3(-2, fix, 2), XMFLOAT2(X0, Z1), XMFLOAT3(0, fix, 0)};
  cubick.at(3) = {XMFLOAT3(2, fix, 2), XMFLOAT2(X1, Z1)}, XMFLOAT3(0, fix, 0);
  fix = 2.0f; // NORTH DIRECTION
  cubick.at(4) = {XMFLOAT3(fix, 2, -2), XMFLOAT2(X0, Z0), XMFLOAT3(fix, 0, 0)};
  cubick.at(5) = {XMFLOAT3(fix, 2, 2), XMFLOAT2(X0, Z1), XMFLOAT3(fix, 0, 0)};
  cubick.at(6) = {XMFLOAT3(fix, -2, -2), XMFLOAT2(X1, Z0), XMFLOAT3(fix, 0, 0)};
  cubick.at(7) = {XMFLOAT3(fix, -2, 2), XMFLOAT2(X1, Z1), XMFLOAT3(fix, 0, 0)};
  fix = -2.0f; // SOUTH DIRECTION
  cubick.at(8) = {XMFLOAT3(fix, 2, -2), XMFLOAT2(X0, Z0), XMFLOAT3(fix, 0, 0)};
  cubick.at(9) = {XMFLOAT3(fix, 2, 2), XMFLOAT2(X0, Z1), XMFLOAT3(fix, 0, 0)};
  cubick.at(10) = {XMFLOAT3(fix, -2, -2), XMFLOAT2(X1, Z0), XMFLOAT3(fix, 0, 0)};
  cubick.at(11) = {XMFLOAT3(fix, -2, 2), XMFLOAT2(X1, Z1), XMFLOAT3(fix, 0, 0)};
  fix = 2.0f; // EAST DIRECTION
  cubick.at(12) = {XMFLOAT3(2, 2, fix), XMFLOAT2(X0, Z0), XMFLOAT3(0, 0, fix)};
  cubick.at(13) = {XMFLOAT3(-2, 2, fix), XMFLOAT2(X0, Z1), XMFLOAT3(0, 0, fix)};
  cubick.at(14) = {XMFLOAT3(2, -2, fix), XMFLOAT2(X1, Z0), XMFLOAT3(0, 0, fix)};
  cubick.at(15) = {XMFLOAT3(-2, -2, fix), XMFLOAT2(X1, Z1), XMFLOAT3(0, 0, fix)};
  fix = -2.0f; // WEST DIRECTION
  cubick.at(16) = {XMFLOAT3(2, 2, fix), XMFLOAT2(X0, Z0), XMFLOAT3(0, 0, fix)};
  cubick.at(17) = {XMFLOAT3(-2, 2, fix), XMFLOAT2(X0, Z1), XMFLOAT3(0, 0, fix)};
  cubick.at(18) = {XMFLOAT3(2, -2, fix), XMFLOAT2(X1, Z0), XMFLOAT3(0, 0, fix)};
  cubick.at(19) = {XMFLOAT3(-2, -2, fix), XMFLOAT2(X1, Z1), XMFLOAT3(0, 0, fix)};
  fix = -2.0f; // DOWN DIRECTON
  cubick.at(20) = {XMFLOAT3(-2, fix, -2), XMFLOAT2(X0, Z0), XMFLOAT3(0, fix, 0)};
  cubick.at(21) = {XMFLOAT3(2, fix, -2), XMFLOAT2(X0, Z1), XMFLOAT3(0, fix, 0)};
  cubick.at(22) = {XMFLOAT3(-2, fix, 2), XMFLOAT2(X1, Z0), XMFLOAT3(0, fix, 0)};
  cubick.at(23) = {XMFLOAT3(2, fix, 2), XMFLOAT2(X1, Z1), XMFLOAT3(0, fix, 0)};

  return cubick;
}

indices cub_idx() {
  return indices(
      {1, 0, 3, 0, 2, 3, 4, 5, 7, 6, 4, 7, 9, 8, 11, 8, 10, 11, 12, 13, 15, 14, 12, 15, 17, 16, 19, 16, 18, 19, 21, 23, 20, 23, 22, 20});
}


vertices pyramid_verts() {
  vertices cubick(16);
  float fix = 2.0f; // NORTH DIRECTION
  cubick.at(0) = {XMFLOAT3(fix, -2, -2), XMFLOAT2(X0, Z0)};
  cubick.at(1) = {XMFLOAT3(0, 2, 0), XMFLOAT2(X0, Z1)};
  cubick.at(2) = {XMFLOAT3(fix, -2, 2), XMFLOAT2(X1, Z1)};
  fix = -2.0f; // SOUTH DIRECTION
  cubick.at(3) = {XMFLOAT3(fix, -2, -2), XMFLOAT2(X0, Z0)};
  cubick.at(4) = {XMFLOAT3(0, 2, 0), XMFLOAT2(X0, Z1)};
  cubick.at(5) = {XMFLOAT3(fix, -2, 2), XMFLOAT2(X1, Z1)};
  fix = 2.0f; // EAST DIRECTION
  cubick.at(6) = {XMFLOAT3(-2, -2, fix), XMFLOAT2(X0, Z0)};
  cubick.at(7) = {XMFLOAT3(0, 2, 0), XMFLOAT2(X0, Z1)};
  cubick.at(8) = {XMFLOAT3(2, -2, fix), XMFLOAT2(X1, Z1)};
  fix = -2.0f; // WEST DIRECTION
  cubick.at(9) = {XMFLOAT3(-2, -2, fix), XMFLOAT2(X0, Z0)};
  cubick.at(10) = {XMFLOAT3(0, 2, 0), XMFLOAT2(X0, Z1)};
  cubick.at(11) = {XMFLOAT3(2, -2, fix), XMFLOAT2(X1, Z1)};
  fix = -2.0f; // DOWN DIRECTON
  cubick.at(12) = {XMFLOAT3(-2, fix, -2), XMFLOAT2(X0, Z0)};
  cubick.at(13) = {XMFLOAT3(2, fix, -2), XMFLOAT2(X0, Z1)};
  cubick.at(14) = {XMFLOAT3(-2, fix, 2), XMFLOAT2(X1, Z0)};
  cubick.at(15) = {XMFLOAT3(2, fix, 2), XMFLOAT2(X1, Z1)};

  return cubick;
}

indices pyramid_idx() {
  return indices(
      {0, 1, 2, 4, 3, 5, 7, 6, 8, 9, 10, 11, 13, 15, 12, 15, 14, 12});
}