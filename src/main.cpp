#include <thread>
#pragma once

#include <cstdlib>
#include <ctime>
#include "../include/App.hpp"
#include "../include/Math.hpp"

int main() {
    App app;

    std::jthread audioThread([](std::stop_token st) {
        playRandomSong(st);
    });

    app.run();

    return 0;
    std::srand(static_cast<unsigned>(time(0)));
    app.board.soundLoopRunning = false;
    app.board.soundThread.join();
}