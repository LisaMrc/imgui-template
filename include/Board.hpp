#pragma once

#include <imgui.h>
#include <memory>
#include <vector>
#include "Math.hpp"
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
    void                                update(int, int);
    void                                draw();
    void                                move(int, int);
    bool                                IsValidMove(Piece* piece, int row, int col);
    bool                                isPathClear(Piece* piece, int destRow, int destCol);
    Piece*                              getPieceAt(int row, int col);
    void                                removePiece(Piece* piece);
    void                                performCastle(King* king, int destRow, int destCol);
    bool                                isKingInCheck(King*);
    bool                                canHighlightSquares(Piece* selectedPiece, bool isWhite, int row, int col);
    void                                highlightSquares(ImVec2, ImVec2, int, int);
    void                                drawPieces();
    void                                promotePawn(char newSymbol);
    bool                                checkPromotion();
    void                                showPromotionWindow();
    ImFont*                             getFont();
    void                                setFont(ImFont*);

private:
    Piece*              selectedPiece = nullptr;
    Player*             activePlayer  = &white;
    bool                whiteTurn     = true;
    std::vector<Piece*> kings;

    bool   isInCheck          = false;
    bool   isPromotionPending = false;
    int    promotionTargetRow, promotionTargetCol;
    Piece* promotedPawn = nullptr;

    ImFont* customFont  = nullptr;
    ImFont* defaultFont = nullptr;

    ImDrawList* draw_list;
    ImVec2      window_pos;
    ImVec2      window_size;

    float  board_size;
    float  tile_size;
    ImVec2 p;

    ImU32 lightSquare;
    ImU32 darkSquare;
    ImU32 highlight_color;
    ImU32 check_color;

    Tools       tools;
    Binomial    binomial;
    Exponential exp;

    float moveCount = 0;
};