#include <thread>
#include "../include/App.hpp"
#include "../include/Math.hpp"

int main() {
    App app;

    std::jthread audioThread([](std::stop_token st) {
        playRandomSong(st);
    });

    app.run();

    return 0;
}