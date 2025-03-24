#pragma once

class Player {
private:
    // TODO(🚀) : use this bool to shorten removed king detection
    // bool                                   didTheyWin = false;
    bool isWhite = true;

public:
    explicit Player(bool isW)
        : isWhite(isW) {}

    bool                                   getColor() const { return isWhite; }
    void                                   setColor(bool newColor);
};