#include <iostream>

enum class whoPlays {
  white,
  black,
};

class App {
public:
  // Constructor
  App() { std::cout << "Chess App Initialized" << std::endl; }

  // Start the application
  void run() {
    std::cout << "Welcome to the Chess App!" << std::endl;
    // Add game loop or menu logic here
  }

private:
  // Add member variables for game state, board, and players
  // Example:
  // ChessBoard board;
  // Player player1, player2;
};
