#pragma once

#include "Board.hpp"
#include "Player.hpp"

class App {
public:
  // Constructor
  App() = default;

  void initializePlayer();
  void setRounds();

  void update();

//   TODO (lisam) : create destructor

private:
  Player playerWhite, playerBlack;
  Player* whoPlays = &playerWhite;
};
