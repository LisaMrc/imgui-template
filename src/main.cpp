#include <thread>
#include "../include/App.hpp"
#include "../include/Math.hpp"
#include <GLFW/glfw3.h>

int main()
{
    if (!glfwInit()) {
        std::cerr << "Erreur fatale : impossible d'initialiser GLFW." << std::endl;
        return -1;
    }

    App app;

    std::jthread audioThread([](std::stop_token st) {
        playRandomSong(st);
    });

    app.run();

    glfwTerminate(); // Nettoyage GLFW
    return 0;
}