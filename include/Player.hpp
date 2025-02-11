#pragma once

#include <array>
#include "Pieces.hpp"

class Player {
private:
    // std::array<std::unique_ptr<Piece>, 16> playerPieces;
    // bool                                   didTheyWin = false;
    bool isWhite = true;

public:
    explicit Player(bool isW)
        : isWhite(isW) {}

    bool getColor() const { return isWhite; }
    void setColor(bool newColor);
};