#pragma once

#include <imgui.h>
#include "../include/App.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    App app{};

    quick_imgui::loop(
        "Chess",
        /* init: */ [&]() {
            ImGuiIO& io = ImGui::GetIO();
            io.Fonts->AddFontFromFileTTF("D:/Jean/docs/travail/IMAC/Semestre 4/Programmation/imgui-template/assets/font/Chess-7.TTF", 16.0f); // Assurez-vous de mettre le bon chemin vers la police
        },
        /* loop: */
        [&]() { app.update(); }
    );
}