#pragma once

#include <array>
#include "Pieces.hpp"

class Player {
private:
    bool didTheyWin = false;
    bool isWhite    = true;

public:
    Player() = default;

    std::array<std::unique_ptr<Piece>, 16> playerPieces;
    bool                                   getColor() const { return isWhite; }
    void                                   setColor(bool newColor);
};