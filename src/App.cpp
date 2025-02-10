#pragma once

#include <imgui.h>

#include "../include/App.hpp"
#include "../include/Board.hpp"

App::App()
{
    initializePlayerColor(playerWhite, true);
    initializePlayerColor(playerBlack, false);
    initializePieces(playerWhite);
    initializePieces(playerBlack);
}

void App::update()
{
    ImGui::ShowDemoWindow(); // This opens a window which shows tons of
                             // examples of what you can do with ImGui. You
                             // should check it out! Also, you can use the
                             // "Item Picker" in the top menu of that demo
                             // window: then click on any widget and it will
                             // show you the corresponding code directly in
                             // your IDE!

    ImGui::Begin("Chess");
    draw_board();
    ImGui::End();

    // TODO(lisam : define a condition where if playerX'king is out of the board, (isOnBoard=0), game stops and displays victory message
    // if (playerWhite.playerPieces[0]) {
    
    // }
}

void App::initializePlayerColor(Player& player, bool newColor)
{
    player.setColor(newColor);
}

void App::initializePieces(Player& player)
{
    if (player.getColor())
    {
        player.playerPieces[0] = std::make_unique<King>(Position(5, 1));
        player.playerPieces[1] = std::make_unique<Queen>(Position(3, 1));

        player.playerPieces[2] = std::make_unique<Bishop>(Position(3, 1));
        player.playerPieces[3] = std::make_unique<Bishop>(Position(6, 1));

        player.playerPieces[4] = std::make_unique<Knight>(Position(2, 1));
        player.playerPieces[5] = std::make_unique<Knight>(Position(7, 1));

        player.playerPieces[6] = std::make_unique<Rook>(Position(1, 1));
        player.playerPieces[7] = std::make_unique<Rook>(Position(8, 1));

        for (int i = 8; i < 16; i++)
        {
            player.playerPieces[i] = std::make_unique<Pawn>(Position(i - 7, 2));
        }
    }
    else
    {
        player.playerPieces[0] = std::make_unique<King>(Position(5, 8));
        player.playerPieces[1] = std::make_unique<Queen>(Position(3, 8));

        player.playerPieces[2] = std::make_unique<Bishop>(Position(3, 8));
        player.playerPieces[3] = std::make_unique<Bishop>(Position(6, 8));

        player.playerPieces[4] = std::make_unique<Knight>(Position(2, 8));
        player.playerPieces[5] = std::make_unique<Knight>(Position(7, 8));

        player.playerPieces[6] = std::make_unique<Rook>(Position(1, 8));
        player.playerPieces[7] = std::make_unique<Rook>(Position(8, 8));

        for (int i = 8; i < 16; i++)
        {
            player.playerPieces[i] = std::make_unique<Pawn>(Position(i - 7, 7));
        }
    }
}