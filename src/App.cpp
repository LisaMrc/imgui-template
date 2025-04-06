#pragma once

#include "../include/App.hpp"
#include "../include/Board.hpp"
#include "../include/Player.hpp"

void App::update()
{
    ImGui::PushFont(customFont);

    ImGui::Begin("Chess");
    board.draw();
    ImGui::End();

    ImGui::PopFont();
}

void App::init()
{
    // playerWhite.setColor(true);
    // playerBlack.setColor(false);
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    customFont  = io.Fonts->AddFontFromFileTTF("../../font/CHEQ_TT.TTF", 40.0f);

    board.init();
}