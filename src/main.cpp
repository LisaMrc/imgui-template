#include <thread>
#include "../include/App.hpp"
#include "../include/Math.hpp"

int main()
{
    // glmax::Shader myShader;
    // myShader.load_shader("../bin/src/shaders/basic.vert", "../bin/src/shaders/basic.frag");
    // if (!myShader.is_valid())
    // {
    //     std::cerr << "Erreur fatale : le shader n'a pas pu être compilé." << std::endl;
    //     return -1;
    // }

    App app;
    // app.setShader(&myShader);

    std::jthread audioThread([](std::stop_token st) {
        playRandomSong(st);
    });

    app.run();

    return 0;
}