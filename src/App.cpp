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
    board.wasKingRemoved();

    ImGui::End();
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
        /* init: */
        [&]() {
            glEnable(GL_DEPTH_TEST); // Permet le rendu 3D correct
        },
        [&]() {
            glClearColor(1, 0, 1, 1); // Principal window
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display3DObj(); // Rendu 3D

            update();
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

}