enum class piece_type {
  pawn,
  rook,
  knight,
  bishop,
  king,
  queen,
};

struct position {
  int x{};
  int y{};
};

class piece {
private:
  piece_type type{};
  position position{};

public:
  void range();
};