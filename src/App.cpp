#pragma once

#include "../include/App.hpp"
#include <imgui.h>
#include "../include/Board.hpp"
#include "../include/Render.hpp"
#include "glad/glad.h"
#include "quick_imgui/quick_imgui.hpp"

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
    board.debug_removeWhiteKingButton();
    board.debug_removeBlackKingButton();
    displayGameOverScreen();
    ImGui::End();

    render3DObj("../../Assets/Objects/Pieces/Pawn.obj", 0, 0);
    // render3DPieces();
}

void App::init()
{
    board.init();
}

void App::run()
{
    init();

    quick_imgui::loop(
        "Zen Chess",
        {
            .init = [&]() 
            {
                glEnable(GL_DEPTH_TEST); // Enables correct 3D rendering
            },
            .loop = [&]() 
            {
                glClearColor(1, 0, 1, 1); // Principal window
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                update(); 
            },

            .mouse_button_callback = [&](int button, int action, int mods) 
            {
                // Handle mouse clicks if needed
            }
        }
    );
}

void App::handleEvent()
{
    // TODO(lisam) : enable mouse trigger for trackball camera
    if (true)
    {
        TrackBallCamera.moveFront(.001);
    }
}

void App::displayGameOverScreen()
{
    if (board.wasWhiteKingRemoved())
    {
        ImGui::SetNextWindowPos(ImVec2(400, 300), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 150));
        ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        ImGui::Text("Game Over!");
        ImGui::Text("White king has been captured. Black wins !");

        ImGui::End();
    }

    if (board.wasBlackKingRemoved())
    {
        ImGui::SetNextWindowPos(ImVec2(400, 300), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 150));
        ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        ImGui::Text("Game Over!");
        ImGui::Text("Black king has been captured. White wins !");

        ImGui::End();
    }
}