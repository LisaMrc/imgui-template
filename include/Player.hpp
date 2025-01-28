#pragma once

#include "Pieces.hpp"

#include <array>

class Player {
private:
  std::array<std::unique_ptr<Piece>, 16> playerPieces;
  bool didTheyWin = false;

public:
//   Player();
};