#pragma once

#include <memory>
#include <vector>
#include "Piece.hpp"
#include "Player.hpp"

class Board {
private:
    Player white;
    Player black;

public:
    Board()
        : white(true), black(false) {}
    std::vector<std::unique_ptr<Piece>> pieces;
    void                                init();
    void                                update();
    void                                draw();
    bool                                IsValidMove(Piece* piece, int row, int col);
    bool                                isPathClear(Piece* piece, int destRow, int destCol);
    Piece*                              getPieceAt(int row, int col);
    void                                removePiece(Piece* piece);
    void                                performCastle(King* king, int destRow, int destCol);
    bool                                isKingInCheck(King*);
    bool                                highlightSquares(Piece* selectedPiece, bool isWhite, int row, int col);

    Piece*  selectedPiece = nullptr;
    Player* activePlayer  = &white;
    bool    whiteTurn     = true;
    bool    isInCheck     = false;
};