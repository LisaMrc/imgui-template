#pragma once

#include "Board.hpp"
#include "Player.hpp"

class App {
public:
    App();
    void update();

private:
    int     RoundNbr = 0;
    Player  playerWhite;
    Player  playerBlack;
    Player* whoPlays = &playerWhite;

    void initializePlayerColor(Player& player, bool newColor);
    void initializePieces(Player& player);
};
