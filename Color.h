#ifndef PIZZA_COLOR
#define PIZZA_COLOR

#include <cstdint>

struct Color {
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

// Operator needed for std::map usage
inline bool operator<(const Color &lhs, const Color &rhs) {
  if (lhs.r < rhs.r)
    return true;
  if (lhs.g < rhs.g)
    return true;
  if (lhs.b < rhs.b)
    return true;
  return false;
}

#endif