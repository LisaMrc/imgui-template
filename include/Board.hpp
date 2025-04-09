#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include "Math.hpp"
#include "Piece.hpp"
#include "Player.hpp"
#include "Stockfish.hpp"
#include "imgui.h"

struct Position {
    int row;
    int col;
};

class Board {
private:
public:
    Player white;
    Player black;
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
    void                                playAI();
    std::string                         toChessNotation(int row, int col);
    Position                            chessNotationToIndices(const std::string& notation);
    StockfishEngine                     stockfish;

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
    Gamma       gamma;
    Bernoulli   bernoulli;  // Add Bernoulli as a member

    float moveCount = 0;

    std::vector<std::string> movesPlayed;
    bool                     AImode = true;


    void SwitchPlayer(double probability)
    {
        if (bernoulli.flip(probability)) // Calls the flip method from Bernoulli class
        {
            activePlayer = (activePlayer->getColor()) ? &black : &white;

            std::cout << "⚡ Player SWITCHED! Now it's "
                      << (activePlayer->getColor() ? "White" : "Black")
                      << "'s turn!" << '\n';
        }
    }
};