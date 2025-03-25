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
        {
            .init = [&]() {
                glEnable(GL_DEPTH_TEST); // Permet le rendu 3D correct
            },
            .loop                     = [&]() {
            glClearColor(1, 0, 1, 1); // Principal window
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            display3DObj(); // Rendu 3D
            handleInput(); 
            update(); 
        
        },
            .key_callback             = [](int key, int scancode, int action, int mods) { std::cout << "Key: " << key << " Scancode: " << scancode << " Action: " << action << " Mods: " << mods << '\n'; },
            .mouse_button_callback    = [](int button, int action, int mods) { std::cout << "Button: " << button << " Action: " << action << " Mods: " << mods << '\n'; },
            .cursor_position_callback = [](double xpos, double ypos) { std::cout << "Position: " << xpos << ' ' << ypos << '\n'; },
            .scroll_callback          = [](double xoffset, double yoffset) { std::cout << "Scroll: " << xoffset << ' ' << yoffset << '\n'; },
            .window_size_callback     = [](int width, int height) { std::cout << "Resized: " << width << ' ' << height << '\n'; },
        }
    );
}

// void App::handleEvent()
// {
//     // TODO(lisam) : enable mouse trigger for trackball camera
//     if (true)
//     {
//         TrackBallCamera.moveFront(.001);
//     }
// }

void App::displayGameOverScreen()
{
}

void App::handleInput() {
//     if (isKeyPressed(GLFW_KEY_C)) {  // Change de mode caméra avec la touche C
//         isTrackball = !isTrackball;
//         camera.switchMode(isTrackball);
//     }

//     if (isTrackball) {
//         if (isKeyPressed(GLFW_KEY_W)) camera.trackball.moveFront(0.1f);
//         if (isKeyPressed(GLFW_KEY_A)) camera.trackball.rotateLeft(-5.0f);
//         if (isKeyPressed(GLFW_KEY_D)) camera.trackball.rotateLeft(5.0f);
//         if (isMouseMoving()) camera.trackball.handleMouseMotion(getMouseDX(), getMouseDY());
//     } else {
//         if (isKeyPressed(GLFW_KEY_W)) camera.freefly.moveFront(0.1f);
//         if (isKeyPressed(GLFW_KEY_A)) camera.freefly.moveLeft(0.1f);
//         if (isKeyPressed(GLFW_KEY_D)) camera.freefly.moveRight(0.1f);
//         if (isMouseMoving()) camera.freefly.handleMouseMotion(getMouseDX(), getMouseDY());
//     }
}