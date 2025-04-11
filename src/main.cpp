#include <thread>
#include "../include/App.hpp"
#include "../include/Audio.hpp"

int main()
{
    App         app;
    AudioEngine audioEngine;

    std::jthread audioThread([&](std::stop_token st) {
        audioEngine.playRandomSong(st);
    });

    std::jthread soundThread([&](std::stop_token st) {
        audioEngine.playRandomSound(st, app.board);
    });

    app.run(); // blocks until app is closed

    // Clean shutdown of both threads
    audioThread.request_stop();
    soundThread.request_stop();
    audioThread.join();
    soundThread.join();

    glfwTerminate(); // Nettoyage GLFW
    return 0;
}