#pragma once

#include <imgui.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../include/Piece.hpp"
#include "Math.hpp"
#include "Piece.hpp"
#include "Player.hpp"
#include "Render.hpp"
#include "Stockfish.hpp"
#include "imgui.h"
#include <GLFW/glfw3.h>

struct Position {
    int row;
    int col;
};

class Board {
public:
    Player        white;
    Player        black;
    RenderEngine& renderEngine;
    Board(RenderEngine& t_renderEngine)
        : white(true), black(false), renderEngine(t_renderEngine) {}
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
    ImFont*                             getFont(int what);
    void                                setFont(ImFont*);
    void                                soundLoop();
    void                                playSound();
    void                                playAI();
    std::string                         toChessNotation(int row, int col);
    Position                            chessNotationToIndices(const std::string& notation);
    StockfishEngine                     stockfish;
    // bool                                wasWhiteKingRemoved();
    // bool                                wasBlackKingRemoved();
    // void                                debug_removeWhiteKingButton();
    // void                                debug_removeBlackKingButton();
    bool        gameOver = false;
    Tools       tools;
    Binomial    binomial;
    Exponential exp;
    Gamma       gamma;
    Bernoulli   bernoulli;

    void blockSquare(int row, int col) {
        blocked[row][col] = true;
    }

    void unblockSquare(int row, int col) {
        blocked[row][col] = false;
    }

    bool isBlocked(int row, int col) {
        return blocked[row][col];
    }

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

    float moveCount = 0;

    std::vector<std::string> movesPlayed;
    bool                     AImode = true;

    bool        whitePlayed = false;
    float       currentTime;
    float       stateStartTime = 0.0f;
    const float waitDuration   = 0.1f;

    bool isCastle = false;

    std::optional<std::pair<int, int>> enPassantTarget; // Holds the square that can be captured via en passant

    void SwitchPlayer(double probability);

    bool blocked[8][8] = {{false}};
    bool squareBlocked = false;
};