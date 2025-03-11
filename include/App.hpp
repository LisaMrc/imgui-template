#pragma once

#include <backends/imgui_impl_glfw.h>
#include "Board.hpp"
#include "TrackballCamera.hpp"

class App {
public:
    App() = default;

    void init();
    void update();
    void run();
    void handleEvent();

private:
    int             RoundNbr = 0;
    Board           board;
    TrackballCamera TrackBallCamera;
};