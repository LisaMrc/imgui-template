#include <thread>
#include "../include/App.hpp"
#include "../include/Math.hpp"

int main() {
    App app;

    std::jthread audioThread([](std::stop_token st) {
        playRandomSong(st);
    });

    app.run(); // Runs the game

    // When the game ends, the jthread will be destructed, stopping music
    return 0;
}