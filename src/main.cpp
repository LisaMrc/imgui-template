#pragma once

#include <cstdlib>
#include <ctime>
#include "../include/App.hpp"
#include "quick_imgui/quick_imgui.hpp"


int main()
{
    App app{};
    app.init();

    std::srand(static_cast<unsigned>(time(0)));

    quick_imgui::loop(
        "Chess",
        /* init: */ [&]() {},
        /* loop: */
        [&]() { app.update(); }
    );
}