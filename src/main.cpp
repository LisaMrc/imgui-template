#pragma once

#include "../include/App.hpp"
#include "quick_imgui/quick_imgui.hpp"
#include "../include/debug.hpp"

int main()
{
    App app{};
    app.init();

    quick_imgui::loop(
        "Chess",
        /* init: */ [&]() {},
        /* loop: */
        [&]() { app.update(); }
    );
}