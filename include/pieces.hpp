#pragma once

#include <cmath>
#include <iostream>

enum class PieceType {
    pawn,
    rook,
    knight,
    bishop,
    king,
    queen,
};

struct Position {
    // TODO(lisam): create something to block values from 1 to 8
    int x{};
    int y{};
};

class Piece {
private:
    Position position{};
    bool     isOnBoard = true;

public:
    Piece() = default;
    explicit Piece(Position const& position);
    virtual PieceType getType() const = 0;
    Position          getPosition() const { return position; }
    void              setPosition(const Position& newPosition) { position = newPosition; }

    //   virtual void whereTo();
    virtual void move(Position newPosition) = 0;

    virtual ~Piece() = default;
    //   TODO (lisam) : copy constructor etc = default
};

class King : public Piece {
public:
    explicit King(Position const& position)
    {
        setPosition(position);
    }

    PieceType getType() const override { return PieceType::king; }

    void move(Position newPosition) override
    {
        int dx = std::abs(newPosition.x - getPosition().x);
        int dy = std::abs(newPosition.y - getPosition().y);

        if (dx <= 1 && dy <= 1)
        {
            getPosition() = newPosition;
            std::cout << "King moved to (" << getPosition().x << ", " << getPosition().y << ")"
                      << '\n';
        }
        else
        {
            std::cout << "Invalid move for King" << '\n';
        }
    }
};

class Queen : public Piece {
public:
    explicit Queen(Position const& position)
    {
        setPosition(position);
    }

    PieceType getType() const override { return PieceType::queen; }

    void move(Position newPosition) override
    {
        int dx = std::abs(newPosition.x - getPosition().x);
        int dy = std::abs(newPosition.y - getPosition().y);

        if (dx == dy || dx == 0 || dy == 0)
        {
            getPosition() = newPosition;
            std::cout << "Queen moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
        }
        else
        {
            std::cout << "Invalid move for Queen" << '\n';
        }
    }
};

class Rook : public Piece {
public:
    explicit Rook(Position const& position)
    {
        setPosition(position);
    }

    PieceType getType() const override { return PieceType::rook; }

    void move(Position newPosition) override
    {
        if (newPosition.x == getPosition().x || newPosition.y == getPosition().y)
        {
            getPosition() = newPosition;
            std::cout << "Rook moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
        }
        else
        {
            std::cout << "Invalid move for Rook" << '\n';
        }
    }
};

class Bishop : public Piece {
public:
    explicit Bishop(Position const& position)
    {
        setPosition(position);
    }

    PieceType getType() const override { return PieceType::bishop; }

    void move(Position newPosition) override
    {
        int dx = std::abs(newPosition.x - getPosition().x);
        int dy = std::abs(newPosition.y - getPosition().y);

        if (dx == dy)
        {
            getPosition() = newPosition;
            std::cout << "Bishop moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
        }
        else
        {
            std::cout << "Invalid move for Bishop" << '\n';
        }
    }
};

class Knight : public Piece {
public:
    explicit Knight(Position const& position)
    {
        setPosition(position);
    }

    PieceType getType() const override { return PieceType::knight; }

    void move(Position newPosition) override
    {
        int dx = std::abs(newPosition.x - getPosition().x);
        int dy = std::abs(newPosition.y - getPosition().y);

        if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2))
        {
            getPosition() = newPosition;
            std::cout << "Knight moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
        }
        else
        {
            std::cout << "Invalid move for Knight" << '\n';
        }
    }
};

class Pawn : public Piece {
public:
    explicit Pawn(Position const& position)
    {
        setPosition(position);
    }

    PieceType getType() const override { return PieceType::pawn; }

    void move(Position newPosition) override
    {
        int dy = newPosition.y - getPosition().y;
        int dx = std::abs(newPosition.x - getPosition().x);

        // Pawn moves forward 1 square or 2 squares from the initial position
        if ((dy == 1 && dx == 0) || (dy == 2 && getPosition().y == 1 && dx == 0))
        {
            getPosition() = newPosition;
            std::cout << "Pawn moved to (" << getPosition().x << ", " << getPosition().y << ")" << '\n';
        }
        else
        {
            std::cout << "Invalid move for Pawn" << '\n';
        }
    }
};