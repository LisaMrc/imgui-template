#include "../include/Board.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "../include/Piece.hpp"
#include "imgui.h"

void Board::init()
{
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
}

bool Board::IsValidMove(Piece* piece, int row, int col)
{
    if (!piece || piece->isWhite != whiteTurn)
        return false;

    return piece->canMove(row, col);
}

void Board::draw()
{
    ImDrawList* draw_list   = ImGui::GetWindowDrawList();
    ImVec2      window_pos  = ImGui::GetWindowPos();
    ImVec2      window_size = ImGui::GetWindowSize();

    float  board_size = 800.0f;
    float  tile_size  = board_size / 8.0f;
    ImVec2 p(window_pos.x + (window_size.x - board_size) * 0.5f, window_pos.y + (window_size.y - board_size) * 0.5f);

    ImU32 lightSquare     = IM_COL32(255, 206, 158, 255);
    ImU32 darkSquare      = IM_COL32(209, 139, 71, 255);
    ImU32 highlight_color = IM_COL32(0, 255, 0, 255);

    for (int row = 0; row < 8; ++row)
    {
        for (int col = 0; col < 8; ++col)
        {
            ImVec2 min(p.x + col * tile_size, p.y + row * tile_size);
            ImVec2 max(min.x + tile_size, min.y + tile_size);
            draw_list->AddRectFilled(min, max, (row + col) % 2 == 0 ? lightSquare : darkSquare);

            if (selectedPiece && selectedPiece->row == row && selectedPiece->col == col)
            {
                draw_list->AddRect(min, max, highlight_color, 0.0f, 0, 3.0f);
            }

            ImGui::SetCursorScreenPos(min);
            if (ImGui::Button(("##tile" + std::to_string(row) + std::to_string(col)).c_str(), ImVec2(tile_size, tile_size)))
            {
                if (selectedPiece && IsValidMove(selectedPiece, row, col))
                {
                    selectedPiece->row = row;
                    selectedPiece->col = col;
                    whiteTurn          = !whiteTurn;
                    selectedPiece      = nullptr;
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
        }
    }

    // Draw pieces
    for (auto& piece : Board::pieces)
    {
        ImVec2 piece_pos(p.x + piece->col * tile_size + tile_size * 0.35f, p.y + piece->row * tile_size + tile_size * 0.35f);
        char   symbol = piece->getSymbol();
        if (!piece->isWhite)
            symbol = tolower(symbol);

        draw_list->AddText(piece_pos, IM_COL32(0, 0, 0, 255), std::string(1, symbol).c_str());
    }
}
