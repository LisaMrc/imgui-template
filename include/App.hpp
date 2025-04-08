#pragma once

// #include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
#include "Board.hpp"
#include "FreeflyCamera.hpp"
#include "Render.hpp"
#include "Skybox.hpp"
#include "TrackballCamera.hpp"
#include "Camera.hpp"
// #include "glad/glad.h"

class App {
public:
    App() = default;

    void init();
    void update();
    void handleEvent();
    void displayGameOverScreen();
    void run();

private:
    Board           board;
    Camera camera;
    RenderEngine    renderEngine;
    Skybox          skybox;
};

// enum class CameraMode { Freefly,
//                         Trackball };
// extern CameraMode currentCameraMode = CameraMode::Trackball;
