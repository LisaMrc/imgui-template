#pragma once

#include "../include/App.hpp"
#include "../include/Board.hpp"
#include "../include/Render.hpp"
#include "glad/glad.h"
#include "quick_imgui/quick_imgui.hpp"

void App::init()
{
    board.init();
    
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    board.setFont(io.Fonts->AddFontFromFileTTF("../../font/CHEQ_TT.TTF", 40.0f));

    renderEngine.loadShader();
    renderEngine.loadMeshes();
    renderEngine.create3DObj(board);

    float aspect = 800.0f / 600.0f;
    // TODO (🪟) : get window dimensions dynamically

    renderEngine.projectionMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void App::update()
{
    ImGui::PushFont(board.getFont());

    ImGui::Begin("Chess");
    board.draw();
    board.debug_removeWhiteKingButton();
    board.debug_removeBlackKingButton();
    displayGameOverScreen();
    ImGui::End();

    renderEngine.viewMatrix = glm::lookAt(
        glm::vec3(4, 6, 4), // position caméra
        glm::vec3(0, 0, 0), // point visé
        glm::vec3(0, 1, 0)  // axe vertical
    );

    renderEngine.renderAll();

    ImGui::PopFont();
}

void App::handleEvent()
{
    // TODO(🎥) : enable mouse trigger for trackball camera
    // if (true)
    // {
    //     TrackBallCamera.moveFront(.001);
    // }
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

void App::run()
{
    quick_imgui::loop(
        "Zen Chess",
        {.init     = [&]() {
            glEnable(GL_DEPTH_TEST); // Enables correct 3D rendering
            init(); },
         .loop     = [&]() {
                glClearColor(1, 0, 1, 1); // Principal window
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                update(); },
         .shutdown = [&]() {
             renderEngine.cleanUp(); // Nettoyer les buffers à la fin
         },
         .mouse_button_callback = [&](int button, int action, int mods) {
             // Handle mouse clicks if needed
         }}
    );
}