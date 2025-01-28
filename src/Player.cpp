#include "../include/Player.hpp"
#include <memory>

// This functions creates a player and defines its pieces
Player::Player()
{
    playerPieces[0] = std::make_unique<King>();
    playerPieces[1] = std::make_unique<Queen>();

    playerPieces[2] = std::make_unique<Bishop>();
    playerPieces[3] = std::make_unique<Bishop>();

    playerPieces[4] = std::make_unique<Knight>();
    playerPieces[5] = std::make_unique<Knight>();

    playerPieces[6] = std::make_unique<Rook>();
    playerPieces[7] = std::make_unique<Rook>();

    for (int i = 8; i < 16; i++)
    {
        playerPieces[i] = std::make_unique<Pawn>();
    }
}