#pragma once

#include "Board.hpp"

class App {
public:
  // Constructor
  App() = default;

  // Start the application
  void update();

private:
  Chessboard board;
  int round = 1;
//   Player player1, player2;
};
