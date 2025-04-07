#pragma once

#include <imgui.h>
#include "Board.hpp"

class App {
public:
    App() = default;

    void init();
    void update();

    Board board;

private:
    int RoundNbr = 0;
};
