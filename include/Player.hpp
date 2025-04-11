#pragma once

class Player {
private:
    bool isWhite = true;

public:
    explicit Player(bool isW)
        : isWhite(isW) {}

    bool                                   getColor() const { return isWhite; }
    void                                   setColor(bool newColor);
};