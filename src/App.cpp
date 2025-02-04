#pragma once

#include "../include/App.hpp"
#include "../include/Board.hpp"
#include "../include/Player.hpp"

#include <imgui.h>


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
}

void App::initializePlayers()
{
    playerWhite.setColor(true);
    playerBlack.setColor(false);
}

void initializeBlackPieces(Player player)
{
    // if (getColor()) {
    
    // }
}