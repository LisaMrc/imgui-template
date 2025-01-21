#include <iostream>
#include "color.hpp"

class piece {
private:
  int range{};
  color color{};

public:
  void movement();
};