#include "glad/glad.h"
//
#include <imgui.h>
#include "../include/App.hpp"
#include "../include/debug.hpp"
#include "GLFW/glfw3.h"
#include "quick_imgui/quick_imgui.hpp"

int main()
{
    App app{};
    app.init();


    quick_imgui::loop(
        "Chess",
        /* init: */
        [&]() {
            glEnable(GL_DEPTH_TEST); // Permet le rendu 3D correct
        },
        [&]() {
            glClearColor(1, 1, 0, 1);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            display3DObj();  // Rendu 3D

            app.update();
        }
    );

    return 0;
}
