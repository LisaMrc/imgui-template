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
  int x{};
  int y{};
};

class Piece {
private:
  Position position{};
  bool isOnBoard = true;

public:
  virtual PieceType getType() const = 0;

  Position getPosition() const { return position; }
  void setPosition(const Position &newPosition) { position = newPosition; }

  virtual void whereTo();
  virtual void move(Position newPosition) = 0;

  virtual ~Piece() = default;
  //   TODO (lisam) : copy constructor etc = default
};

class King : public Piece {
public:
  PieceType getType() const override { return PieceType::king; }

  void move(Position newPosition) override {
    int dx = std::abs(newPosition.x - getPosition().x);
    int dy = std::abs(newPosition.y - getPosition().y);

    if (dx <= 1 && dy <= 1) {
      getPosition() = newPosition;
      std::cout << "King moved to (" << getPosition().x << ", " << getPosition().y << ")"
                << '\n';
    } else {
      std::cout << "Invalid move for King" << '\n';
    }
  }
};