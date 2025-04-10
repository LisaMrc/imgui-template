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
#include "../include/Piece.hpp"
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
    void                                soundLoop();
    void                                playSound();
    void                                playAI();
    std::string                         toChessNotation(int row, int col);
    Position                            chessNotationToIndices(const std::string& notation);
    std::atomic<bool>                   soundLoopRunning = true;
    std::thread                         soundThread;
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

    float moveCount = 0;

    std::vector<std::string> movesPlayed;
    bool                     AImode = false;

    bool        whitePlayed = false;
    float       currentTime;
    float       stateStartTime = 0.0f;
    const float waitDuration   = 0.1f;

    bool isCastle = false;

    std::optional<std::pair<int, int>> enPassantTarget; // Holds the square that can be captured via en passant
};