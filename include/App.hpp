#pragma once

#include "Board.hpp"

#include <iostream>

class App {
public:
  // Constructor
  App() { std::cout << "Chess App Initialized" << std::endl; }

  // Start the application
  void update();

private:
  // Add member variables for game state, board, and players
  // Example:
  // ChessBoard board;
  // Player player1, player2;
};
