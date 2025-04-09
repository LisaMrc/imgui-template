#include <cstdlib>
#include <ctime>
#include <thread>
#include "../include/App.hpp"
#include "../include/Math.hpp"

int main()
{
    App app;

    std::jthread audioThread([](std::stop_token st) {
        playRandomSong(st);
    });

    app.run();

    std::srand(static_cast<unsigned>(time(nullptr)));
    app.board.soundLoopRunning = false;
    app.board.soundThread.join();

    return 0;
}