#include "../include/App.hpp"
#include "../include/Board.hpp"
#include "../include/Render.hpp"
#include "glad/glad.h"
#include "quick_imgui/quick_imgui.hpp"

void App::init()
{
    board.init();
    renderEngine.loadShader();
    glEnable(GL_DEPTH_TEST);
}

void App::update()
{
    ImGui::ShowDemoWindow();

    ImGui::Begin("Chess");
    board.draw();
    board.debug_removeWhiteKingButton();
    board.debug_removeBlackKingButton();
    displayGameOverScreen();
    ImGui::End();

    // renderEngine.render3DObj("../../Assets/Objects/Pieces/Pawn.obj", 0, 0, renderEngine.shaderProgram);
    // render3DPieces();
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

void App::run()
{
    init(); // => dedans, tu peux mettre glEnable()

    quick_imgui::loop("Zen Chess", quick_imgui::Callbacks{
        .init = [&]() {
            glEnable(GL_DEPTH_TEST);
        },
        .loop = [&]() {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            update();
        },
        .mouse_button_callback = [&](int button, int action, int mods) {
            // Mouse handling
        }
    });
}
