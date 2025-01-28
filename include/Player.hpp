#pragma once

#include <array>

#include "pieces.hpp"

class Player {
private:
    std::array<std::unique_ptr<Piece>, 16> playerPieces;
    bool                                   didTheyWin = false;
    bool                                   isWhite    = true;

public:
    Player();
    void setColor(bool newColor);
};