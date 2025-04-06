#pragma once

#include <imgui.h>
#include "Board.hpp"

class App {
public:
    App() = default;

    void init();
    void update();

private:
    int RoundNbr = 0;
    // Player  playerWhite, playerBlack;
    // Player* whoPlays = &playerWhite;
    Board board;

    ImFont* customFont = nullptr;
};
