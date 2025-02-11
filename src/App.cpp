#pragma once

#include "../include/App.hpp"
#include <imgui.h>
#include "../include/Board.hpp"
#include "../include/Player.hpp"

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
    board.draw();
    ImGui::End();

    // if (playerWhite.playerPieces[0]->getStatus() == 0)
    // {
    //     std::cout << "Black Player wins !" << '\n';
    // }
    // else if (playerBlack.playerPieces[0]->getStatus() == 0)
    // {
    //     std::cout << "White Player wins !" << '\n';
    // }
}

void App::init()
{
    // playerWhite.setColor(true);
    // playerBlack.setColor(false);

    board.init();
}