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

    std::vector<std::string> skyboxFaces = {
        "right.jpg",
        "left.jpg",
        "top.jpg",
        "bottom.jpg",
        "front.jpg",
        "back.jpg"
    };

    skybox.init(skyboxFaces);

    renderEngine.loadShader();
    glEnable(GL_DEPTH_TEST);

    GLFWwindow* window = glfwGetCurrentContext(); // 👈 ajoute cette ligne
    int         width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    projection   = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void App::update()
{
    ImGui::Begin("Chess");

    glm::mat4 view = camera.getViewMatrix();
    renderEngine.render3DObj("assets/pawn.obj", 1, 0, view, projection);
    renderEngine.setViewMatrix(view);
    skybox.draw(view, projection);
    camera.updateMatrices();
    board.draw();
    displayGameOverScreen();

    ImGui::End();
}

// void App::handleEvent()
// {
//     GLFWwindow* window = glfwGetCurrentContext();
//     if (!window)
//         return;

//     // Changer de caméra avec la touche C
//     if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
//     {
//         if (!togglePressed)
//         {
//             CameraMode newMode = (camera.getMode() == CameraMode::Trackball)
//                                      ? CameraMode::Freefly
//                                      : CameraMode::Trackball;
//             camera.setMode(newMode);
//             togglePressed = true;
//         }
//     }
//     else
//     {
//         togglePressed = false;
//     }

//     if (camera.getMode() == CameraMode::Trackball)
//     {
//         if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//             camera.moveFront(-0.1f); // Zoom in
//         if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//             camera.moveFront(0.1f); // Zoom out
//         if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//             camera.rotateLeft(-1.0f);
//         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//             camera.rotateLeft(1.0f);
//     }

//     if (camera.getMode() == CameraMode::Freefly)
//     {
//         if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//             camera.rotateUp(-1.0f);
//         if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//             camera.rotateUp(1.0f);
//         if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//             camera.rotateLeft(1.0f);
//         if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//             camera.rotateLeft(-1.0f);
//     }
//     // Contrôles communs à tous les modes (ZQSD)
//     // if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//     //     camera.rotateUp(-1.0f);
//     // if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//     //     camera.rotateUp(1.0f);
//     // if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//     //     camera.rotateLeft(1.0f);
//     // if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//     //     camera.rotateLeft(-1.0f);
// }

void App::handleEvent()
{
    GLFWwindow* window = glfwGetCurrentContext();
    if (!window)
        return;

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        if (!togglePressed)
        {
            CameraMode newMode = (camera.getMode() == CameraMode::Trackball)
                                     ? CameraMode::Freefly
                                     : CameraMode::Trackball;
            camera.setMode(newMode);
            glfwSetInputMode(window, GLFW_CURSOR, camera.getMode() == CameraMode::Freefly ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
            togglePressed   = true;
            firstMouseInput = true;
        }
    }
    else
    {
        togglePressed = false;
    }

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    if (firstMouseInput)
    {
        lastMouseX      = xpos;
        lastMouseY      = ypos;
        firstMouseInput = false;
    }

    float deltaX = static_cast<float>(xpos - lastMouseX);
    float deltaY = static_cast<float>(ypos - lastMouseY);

    lastMouseX = xpos;
    lastMouseY = ypos;

    // TRACKBALL CONTROLS
    if (camera.getMode() == CameraMode::Trackball)
    {
        // Rotation verticale (haut/bas)
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // vers le haut
            camera.rotateUp(-1.0f);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // vers le bas
            camera.rotateUp(1.0f);

        // Rotation horizontale (gauche/droite)
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera.rotateLeft(1.0f);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera.rotateLeft(-1.0f);

        // Zoom avant / arrière
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            camera.moveFront(-0.1f); // Zoom in
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            camera.moveFront(0.1f); // Zoom out

        // Rotation aussi possible avec ← / → si souhaité
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            camera.rotateLeft(-1.0f);
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            camera.rotateLeft(1.0f);
    }

    // FREEFLY CONTROLS
    if (camera.getMode() == CameraMode::Freefly)
    {
        float sensitivity = 0.1f;
        camera.rotateLeft(-deltaX * sensitivity);
        camera.rotateUp(-deltaY * sensitivity);
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
    quick_imgui::loop("Zen Chess", quick_imgui::Callbacks{.init = [&]() {
                                                              if (!gladLoadGL())
                                                              {
                                                                  std::cerr << "Erreur : échec de l'initialisation de Glad." << std::endl;
                                                                  exit(-1);
                                                              }
                                                              glEnable(GL_DEPTH_TEST);
                                                              init(); // Initialisations liées au jeu
                                                          },
                                                          .loop                  = [&]() {
                                                            std::cout << "Loop running\n";
            glClearColor(1, 0, 1, 1); // Violet pour debug visuel
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            handleEvent();
            update(); },
                                                          .mouse_button_callback = [&](int button, int action, int mods) {
                                                              // Gestion souris ici
                                                          }});
}
