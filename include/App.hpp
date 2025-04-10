#pragma once

// #include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui.h>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
#include "Board.hpp"
#include "Camera.hpp"
#include "Render.hpp"
#include "Skybox.hpp"

// #include "glad/glad.h"

class App {
public:
    App() = default;

    void   init();
    void   update();
    void   handleEvent();
    void   displayGameOverScreen();
    void   run();
    bool   isTrackballMode() const { return m_UseTrackballCamera; }
    void   toggleCameraMode() { m_UseTrackballCamera = !m_UseTrackballCamera; }
    double lastMouseX      = 0.0;
    double lastMouseY      = 0.0;
    bool   firstMouseInput = true;

private:
    Board        board;
    Camera       camera;
    RenderEngine renderEngine;
    Skybox       skybox;
    glm::mat4    projection;
    bool         m_UseTrackballCamera = true; // true = trackball, false = freefly
};

// enum class CameraMode { Freefly,
//                         Trackball };
// extern CameraMode currentCameraMode = CameraMode::Trackball;
