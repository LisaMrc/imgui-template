#include "color.hpp"

enum class piece_type {
  pawn,
  tower,
  knight,
  fool,
  king,
  queen,
};
class piece {
private:
  piece_type type{};
  color color{};

public:
  void range();
};