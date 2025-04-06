#include "../include/Board.hpp"
#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../include/Binomial.hpp"
#include "../include/Piece.hpp"
#include "imgui.h"

void Board::init()
{
    board_size = 800.0f;
    tile_size  = board_size / 8.0f;

    lightSquare     = IM_COL32(255, 206, 158, 255);
    darkSquare      = IM_COL32(209, 139, 71, 255);
    highlight_color = IM_COL32(0, 255, 0, 255);
    check_color     = IM_COL32(255, 0, 0, 255);

    pieces.emplace_back(std::make_unique<Pawn>(6, 0, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 1, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 2, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 3, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 4, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 5, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 6, true));
    pieces.emplace_back(std::make_unique<Pawn>(6, 7, true));
    pieces.emplace_back(std::make_unique<Pawn>(1, 0, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 1, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 2, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 3, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 4, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 5, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 6, false));
    pieces.emplace_back(std::make_unique<Pawn>(1, 7, false));

    pieces.emplace_back(std::make_unique<Rook>(7, 0, true));
    pieces.emplace_back(std::make_unique<Rook>(7, 7, true));
    pieces.emplace_back(std::make_unique<Rook>(0, 0, false));
    pieces.emplace_back(std::make_unique<Rook>(0, 7, false));

    pieces.emplace_back(std::make_unique<Knight>(7, 1, true));
    pieces.emplace_back(std::make_unique<Knight>(7, 6, true));
    pieces.emplace_back(std::make_unique<Knight>(0, 1, false));
    pieces.emplace_back(std::make_unique<Knight>(0, 6, false));

    pieces.emplace_back(std::make_unique<Bishop>(7, 2, true));
    pieces.emplace_back(std::make_unique<Bishop>(7, 5, true));
    pieces.emplace_back(std::make_unique<Bishop>(0, 2, false));
    pieces.emplace_back(std::make_unique<Bishop>(0, 5, false));

    pieces.emplace_back(std::make_unique<Queen>(7, 3, true));
    pieces.emplace_back(std::make_unique<Queen>(0, 3, false));

    pieces.emplace_back(std::make_unique<King>(7, 4, true));
    pieces.emplace_back(std::make_unique<King>(0, 4, false));

    for (auto& piece : pieces)
    {
        piece->board = this;
    }

    if (kings.size() < 2)
    {
        kings.emplace_back(pieces[pieces.size() - 1].get());
        kings.emplace_back(pieces[pieces.size() - 2].get());
    }

    ImGuiIO& io = ImGui::GetIO();
    defaultFont = io.Fonts->AddFontFromFileTTF("../../font/ROBOTO.ttf", 20.0f);
}

void Board::move(int row, int col)
{
    if (selectedPiece && IsValidMove(selectedPiece, row, col) && selectedPiece->isWhite == activePlayer->getColor())
    {
        Piece* targetPiece = getPieceAt(row, col);
        if (targetPiece && targetPiece->isWhite != selectedPiece->isWhite)
        {
            removePiece(targetPiece);
        }
        if (!targetPiece || targetPiece->isWhite != selectedPiece->isWhite)
        {
            selectedPiece->row = row;
            selectedPiece->col = col;
            if (selectedPiece->getType() == 'P')
                selectedPiece->firstMove = false;
            activePlayer  = activePlayer == &white ? &black : &white;
            selectedPiece = nullptr;
        }
    }
    else
    {
        for (auto& piece : Board::pieces)
        {
            if (piece->row == row && piece->col == col)
            {
                selectedPiece = piece.get();
                break;
            }
        }
    }
}

bool Board::IsValidMove(Piece* piece, int row, int col)
{
    if (!piece)
        return false;

    Piece* targetPiece = getPieceAt(row, col);

    if (!isPathClear(piece, row, col) && piece->getType() != 'N')
    {
        return false;
    }

    if (piece->getType() == 'P')
        piece->isCapture = (targetPiece && targetPiece->isWhite != piece->isWhite);

    if (piece->getType() == 'K')
    {
        King* king = dynamic_cast<King*>(piece);
        if (king && king->canCastle(row, col))
        {
            performCastle(king, row, col);
            return true;
        }
    }

    return piece->canMove(row, col);
}

bool Board::isPathClear(Piece* piece, int destRow, int destCol)
{
    int startRow = piece->row;
    int startCol = piece->col;

    int rowDirection = (destRow > startRow) ? 1 : (destRow < startRow) ? -1
                                                                       : 0;
    int colDirection = (destCol > startCol) ? 1 : (destCol < startCol) ? -1
                                                                       : 0;

    int r = startRow + rowDirection;
    int c = startCol + colDirection;

    while (r != destRow || c != destCol)
    {
        if (getPieceAt(r, c) != nullptr)
        {
            return false;
        }
        r += rowDirection;
        c += colDirection;

        if (r < 0 || r >= 8 || c < 0 || c >= 8)
            return false;
    }

    return true;
}

Piece* Board::getPieceAt(int row, int col)
{
    for (const auto& piece : pieces)
    {
        if (piece->row == row && piece->col == col)
        {
            return piece.get();
        }
    }
    return nullptr;
}

void Board::removePiece(Piece* piece)
{
    if (!piece)
        return;

    pieces.erase(std::remove_if(pieces.begin(), pieces.end(), [piece](const std::unique_ptr<Piece>& p) {
                     return p.get() == piece;
                 }),
                 pieces.end());
}

void Board::performCastle(King* king, int destRow, int destCol)
{
    int    direction = (destCol > king->col) ? 1 : -1;
    int    rookCol   = (direction == 1) ? 7 : 0;
    Piece* rook      = getPieceAt(king->row, rookCol);

    if (!rook)
        return;

    king->col = destCol;

    rook->col = king->col - direction;

    king->firstMove = false;
    rook->firstMove = false;
}

bool Board::isKingInCheck(King* king)
{
    if (!king)
        return false;

    bool isKingAttacked = false;

    for (auto& piece : pieces)
    {
        if (piece->isWhite != king->isWhite)
        {
            if (IsValidMove(piece.get(), king->row, king->col))
            {
                isKingAttacked = true;
            }
        }
    }

    return isKingAttacked;
}

bool Board::canHighlightSquares(Piece* selectedPiece, bool isWhite, int row, int col)
{
    if (selectedPiece->getType() == 'P')
    {
        int direction = selectedPiece->isWhite ? -1 : 1;

        // Forward movement
        if (selectedPiece->firstMove)
        {
            if (row == selectedPiece->row + 2 * direction && col == selectedPiece->col)
            {
                return true;
            }
        }

        if (row == selectedPiece->row + direction && col == selectedPiece->col)
        {
            return true;
        }

        // Diagonal captures
        if (row == selectedPiece->row + direction && (col == selectedPiece->col + 1 || col == selectedPiece->col - 1))
        {
            Piece* target = getPieceAt(row, col);
            if (target && target->isWhite != selectedPiece->isWhite)
            {
                return true;
            }
        }
    }
    if (selectedPiece->getType() == 'R')
    {
        if ((selectedPiece->row == row || selectedPiece->col == col) && isPathClear(selectedPiece, row, col))
        {
            return true;
        }
    }
    if (selectedPiece->getType() == 'N')
    {
        int dx = abs(selectedPiece->row - row);
        int dy = abs(selectedPiece->col - col);
        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
        {
            return true;
        }
    }
    if (selectedPiece->getType() == 'B')
    {
        if (abs(selectedPiece->row - row) == abs(selectedPiece->col - col) && isPathClear(selectedPiece, row, col))
        {
            return true;
        }
    }
    if (selectedPiece->getType() == 'Q')
    {
        if (((selectedPiece->row == row || selectedPiece->col == col) || // Rook move
             (abs(selectedPiece->row - row) == abs(selectedPiece->col - col)))
            && // Bishop move
            isPathClear(selectedPiece, row, col))
        {
            return true;
        }
    }
    if (selectedPiece->getType() == 'K')
    {
        int dx = abs(selectedPiece->row - row);
        int dy = abs(selectedPiece->col - col);
        if (dx <= 1 && dy <= 1)
        {
            return true;
        }
    }
    return false;
}

void Board::highlightSquares(ImVec2 min, ImVec2 max, int row, int col)
{
    Piece* targetPiece = getPieceAt(row, col);

    if (!targetPiece || targetPiece->isWhite != selectedPiece->isWhite)
    {
        if (selectedPiece->isWhite)
        {
            if (canHighlightSquares(selectedPiece, true, row, col))
            {
                draw_list->AddRect(min, max, highlight_color, 0.0f, 0, 3.0f);
            }
        }
        else
        {
            if (canHighlightSquares(selectedPiece, false, row, col))
            {
                draw_list->AddRect(min, max, highlight_color, 0.0f, 0, 3.0f);
            }
        }
    }
    if (selectedPiece->row == row && selectedPiece->col == col)
    {
        draw_list->AddRect(min, max, highlight_color, 0.0f, 0, 3.0f);
    }
}

void Board::promotePawn(char newSymbol)
{
    if (!promotedPawn)
        return;

    // Add the new piece
    switch (newSymbol)
    {
    case 'Q':
        pieces.emplace_back(std::make_unique<Queen>(promotionTargetRow, promotionTargetCol, promotedPawn->isWhite));
        break;
    case 'R':
        pieces.emplace_back(std::make_unique<Rook>(promotionTargetRow, promotionTargetCol, promotedPawn->isWhite));
        break;
    case 'B':
        pieces.emplace_back(std::make_unique<Bishop>(promotionTargetRow, promotionTargetCol, promotedPawn->isWhite));
        break;
    case 'N':
        pieces.emplace_back(std::make_unique<Knight>(promotionTargetRow, promotionTargetCol, promotedPawn->isWhite));
        break;
    }

    for (auto& piece : pieces)
    {
        piece->board = this;
    }

    // Reset promotion state
    removePiece(promotedPawn);

    promotedPawn       = nullptr;
    isPromotionPending = false;
}

void Board::showPromotionWindow()
{
    std::cout << "Here\n";

    if (ImGui::BeginPopupModal(".", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        if (binomialResult > 0)
        {
            ImGui::PopFont();

            ImGui::PushFont(defaultFont);

            static const std::vector<std::string> quotes = {
                "Believe in your move.",
                "Every square matters.",
                "Even the king started as a pawn.",
                "Patience is power.",
                "Think ahead. Always."
            };

            static int idx = rand() % quotes.size();
            ImGui::Text("%s", quotes[idx].c_str());

            ImGui::PopFont();

            ImGui::PushFont(customFont);
        }

        if (ImGui::Button("q"))
        {
            promotePawn('Q');
            didBinomial = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("r"))
        {
            promotePawn('R');
            didBinomial = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("b"))
        {
            promotePawn('B');
            didBinomial = false;
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("h"))
        {
            promotePawn('N');
            didBinomial = false;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

bool Board::checkPromotion()
{
    for (auto& piece : pieces)
    {
        if (piece.get()->getType() == 'P' && ((piece->isWhite && piece->row == 0) || (!piece->isWhite && piece->row == 7)))
        {
            promotedPawn = piece.get();
            return true;
        }
    }
    return false;
}

void Board::drawPieces()
{
    for (auto& piece : Board::pieces)
    {
        ImVec2 piece_pos(p.x + piece->col * tile_size + tile_size * 0.28f, p.y + piece->row * tile_size + tile_size * 0.28f);
        char   symbol = piece->getSymbol();

        draw_list->AddText(piece_pos, IM_COL32(0, 0, 0, 255), std::string(1, symbol).c_str());
    }
}

void Board::update(int row, int col)
{
    if (ImGui::Button(("##tile" + std::to_string(row) + std::to_string(col)).c_str(), ImVec2(tile_size, tile_size)))
    {
        move(row, col);
    }
}

ImFont* Board::getFont()
{
    return customFont;
}

void Board::setFont(ImFont* font)
{
    customFont = font;
}

void Board::draw()
{
    draw_list   = ImGui::GetWindowDrawList();
    window_pos  = ImGui::GetWindowPos();
    window_size = ImGui::GetWindowSize();
    p           = {window_pos.x + (window_size.x - board_size) * 0.5f, window_pos.y + (window_size.y - board_size) * 0.5f};

    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            ImVec2 min(p.x + col * tile_size, p.y + row * tile_size);
            ImVec2 max(min.x + tile_size, min.y + tile_size);
            draw_list->AddRectFilled(min, max, (row + col) % 2 == 0 ? lightSquare : darkSquare);

            ImGui::SetCursorScreenPos(min);
            update(row, col);

            if (selectedPiece)
            {
                highlightSquares(min, max, row, col);
            }

            for (auto& king : kings)
            {
                if (isKingInCheck(dynamic_cast<King*>(king)) && king && king->row == row && king->col == col)
                {
                    draw_list->AddRect(min, max, check_color, 0.0f, 0, 3.0f);
                }
            }
        }
    }
    if (checkPromotion())
    {
        isPromotionPending = true;
        promotionTargetRow = promotedPawn->row;
        promotionTargetCol = promotedPawn->col;
        ImGui::OpenPopup(".");
        if (!didBinomial)
        {
            didBinomial    = true;
            binomialResult = binom(engine);
        }
        showPromotionWindow();
    }

    drawPieces();
}
