#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui.h>
#include "Board.hpp"
#include "Camera.hpp"
#include "Render.hpp"
#include "Skybox.hpp"

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

    Board board = Board(this->renderEngine);

private:
    Camera       camera;
    RenderEngine renderEngine;
    Skybox       skybox;
    glm::mat4    projection;
    bool         m_UseTrackballCamera = true;
};