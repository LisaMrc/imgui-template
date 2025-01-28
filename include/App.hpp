#pragma once

#include "Board.hpp"
#include "Player.hpp"

class App {
public:
    App() = default;

    void initializePlayers()
    {
        playerWhite.setColor(true);
        playerBlack.setColor(false);
    };

    void placePieces();
    void setRounds();
    void update();

private:
    int     RoundNbr = 0;
    Player  playerWhite, playerBlack;
    Player* whoPlays = &playerWhite;
};
