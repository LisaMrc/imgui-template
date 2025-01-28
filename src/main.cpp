#pragma once

#include "../include/App.hpp"
#include "quick_imgui/quick_imgui.hpp"

int main() {

  App app{};

    quick_imgui::loop(
        "Chess",
        /* init: */ [&]() {},
        /* loop: */
        [&]() { app.update(); }
    );
}