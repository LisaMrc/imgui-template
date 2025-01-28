#include <cmath>
#include <iostream>
#include <glad/glad.h>  // Ou GLEW selon ta config
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

enum class PieceType {
    pawn,
    rook,
    knight,
    bishop,
    king,
    queen,
};

struct Position {
    int x{};
    int y{};
};

class Piece {
private:
    Position     position{};
    bool         isOnBoard = true;
    unsigned int textureID;

public:
    virtual PieceType getType() const = 0;

    Position getPosition() const { return position; }
    void     setPosition(const Position& newPosition) { position = newPosition; }

    virtual void whereTo();
    virtual void move(Position newPosition) = 0;

    virtual ~Piece() = default;

    virtual void loadTexture(const std::string& texturePath);
    //   TODO (lisam) : copy constructor etc = default
};

class King : public Piece {
public:
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

void Piece::loadTexture(const std::string& texturePath) {
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }
    stbi_image_free(data);
}