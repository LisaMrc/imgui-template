#include "../include/App.hpp"
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
        "../../Assets/Skybox/right.jpg",
        "../../Assets/Skybox/left.jpg",
        "../../Assets/Skybox/top.jpg",
        "../../Assets/Skybox/bottom.jpg",
        "../../Assets/Skybox/front.jpg",
        "../../Assets/Skybox/back.jpg"
    };

    skybox.init(skyboxFaces);

    renderEngine.loadShader();
    glEnable(GL_DEPTH_TEST);

    GLFWwindow* window = glfwGetCurrentContext(); // 👈 ajoute cette ligne
    int         width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = static_cast<float>(width) / static_cast<float>(height);
    projection   = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    board.setFont(io.Fonts->AddFontFromFileTTF("../../Assets/Fonts/CHEQ_TT.TTF", 40.0f));

    renderEngine.loadShader();
    renderEngine.loadMeshes();
    renderEngine.create3DObjects();
    renderEngine.link3DObjectsToPieces(board);
    renderEngine.linkMeshesTo3DObjects();

    // float aspect = 800.0f / 600.0f;
    // TODO (🪟) : get window dimensions dynamically

    renderEngine.projectionMatrix = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
}

void App::update()
{
    ImGui::PushFont(board.getFont(0));

    ImGui::Begin("Chess");

    ImGui::PopFont();
    ImGui::PushFont(board.getFont(1));

    glm::mat4 view = camera.getViewMatrix();
    // renderEngine.render3DObj("assets/pawn.obj", 1, 0, view, projection);
    renderEngine.setViewMatrix(view);
    skybox.draw(view, projection);
    camera.updateMatrices();
    board.draw();

    // renderEngine.viewMatrix = glm::lookAt(
    //     glm::vec3(4, 6, 4), // position caméra
    //     glm::vec3(0, 0, 0), // point visé
    //     glm::vec3(0, 1, 0)  // axe vertical
    // );

    renderEngine.renderAll(board);

    ImGui::PopFont();
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
    ImGui::PopFont();
    ImGui::PushFont(board.getFont(0));
    if (board.gameOver)
    {
        ImGui::SetNextWindowPos(ImVec2(400, 300), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(300, 150));
        ImGui::Begin("Game Over", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

        ImGui::Text("The king has been captured!");

        ImGui::End();
    }
    ImGui::PopFont();
    ImGui::PushFont(board.getFont(1));
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
            glClearColor(1, 0, 1, 1); // Violet pour debug visuel
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            handleEvent();
            update(); },
                                                          .mouse_button_callback = [&](int button, int action, int mods) {
                                                              // Gestion souris ici
                                                          }});
}
