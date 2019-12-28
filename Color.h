#ifndef PIZZA_COLOR
#define PIZZA_COLOR

#include <cstdint>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;

  inline bool operator==(const Color &rhs) {
    if (r != rhs.r)
      return false;
    if (g != rhs.g)
      return false;
    if (b != rhs.b)
      return false;
    return true;
  }

  std::string toString() {
    return std::string(1, r) + std::string(1, g) + std::string(1, b);
  }
};

#endif