#ifndef PIZZA_HEADER
#define PIZZA_HEADER

#include <cstdint>

#pragma pack(push, 1) // Need to add so the program reads headers properly
struct PizzaHeader {
  char signature[5] = {'P', 'I', 'Z', 'Z', 'A'};
  uint16_t width;
  uint16_t height;
  uint8_t LZWWordLength;
  uint8_t colorTableAndCRC{0}; // 2 first bits describe type of color table, 6
                               // bits is CRC
};
#pragma pack(pop)

#endif