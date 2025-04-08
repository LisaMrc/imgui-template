#include "../include/App.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../include/Board.hpp"
#include "../include/Render.hpp"
#include "glad/glad.h"
#include "quick_imgui/quick_imgui.hpp"

static bool togglePressed = false;

void App::init()
{
    board.init();
    skybox.init();
    renderEngine.loadShader();
    glEnable(GL_DEPTH_TEST);
}

void App::update()
{
    ImGui::Begin("Chess");

    glm::mat4 view = camera.getViewMatrix();
    renderEngine.setViewMatrix(view);

    board.draw();
    displayGameOverScreen();

    ImGui::End();
}

void App::handleEvent()
{
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window)
        return;

    // Changer de caméra avec la touche C
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        if (!togglePressed)
        {
            CameraMode newMode = (camera.getMode() == CameraMode::Trackball)
                                     ? CameraMode::Freefly
                                     : CameraMode::Trackball;
            camera.setMode(newMode);
            togglePressed = true;
        }
    }
    else
    {
        togglePressed = false;
    }

    // Contrôles communs à tous les modes (ZQSD)
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.rotateUp(-1.0f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.rotateUp(1.0f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.rotateLeft(1.0f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.rotateLeft(-1.0f);
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

    quick_imgui::loop("Zen Chess", quick_imgui::Callbacks{.init = [&]() { glEnable(GL_DEPTH_TEST); }, .loop = [&]() {
            glClearColor(1, 0, 1, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            handleEvent();
            update(); }, .mouse_button_callback = [&](int button, int action, int mods) {
                                                              // Mouse handling
                                                          }});
}
