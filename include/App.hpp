#pragma once

#include "Board.hpp"
#include "Player.hpp"

class App {
public:
    App() = default;

    void initializePlayers();
    void initializeBlackPieces(Player player);
    void update();

private:
    int     RoundNbr = 0;
    Player  playerWhite, playerBlack;
    Player* whoPlays = &playerWhite;
};
