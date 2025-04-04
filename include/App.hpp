#pragma once

#include <backends/imgui_impl_glfw.h>
#include "../include/Camera.hpp"
#include "Board.hpp"
#include "TrackballCamera.hpp"


class App {
public:
    App() = default;

    void init();
    void update();
    void run();
    void handleEvent();
    void displayGameOverScreen();
    void handleInput();
    void display3DObj();

private:
    int    RoundNbr = 0;
    Board  board;
    Camera camera;
    bool   isTrackball = true;
};