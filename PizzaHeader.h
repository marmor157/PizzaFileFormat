#ifndef PIZZA_HEADER
#define PIZZA_HEADER

#include <cstdint>

#pragma pack(push, 1) // Need to add so the program reads headers properly
struct PizzaHeader {
  uint16_t signature{0x4950};
  uint16_t width;
  uint16_t height;
  uint8_t LZWWordLength;
  uint8_t colorTable{0};
};
#pragma pack(pop)

#endif