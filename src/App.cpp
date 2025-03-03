#pragma once

#include "../include/App.hpp"
#include <imgui.h>
#include "../include/Board.hpp"

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

    if (!board.isKingOnBoard())
    {
        ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetCursorScreenPos(ImVec2(center.x - 100, center.y - 50));
        ImGui::Text("Game Over!");
        return;
    }

    ImGui::End();
}

void App::init()
{
    board.init();
}