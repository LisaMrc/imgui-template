#pragma once

#include <array>
#include "Pieces.hpp"

class Player {
private:
    std::array<std::unique_ptr<Piece>, 16> playerPieces;
    bool                                   didTheyWin = false;

public:
    // Player();
};