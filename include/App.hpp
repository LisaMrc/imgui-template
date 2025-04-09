#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Board.hpp"
#include "Render.hpp"
#include "TrackballCamera.hpp"

class App {
public:
    App() = default;

    void init();
    void update();
    void handleEvent();
    void displayGameOverScreen();
    void run();

    Board board;

private:
    TrackballCamera TrackBallCamera;
    RenderEngine    renderEngine;
};