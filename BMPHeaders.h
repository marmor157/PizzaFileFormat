#ifndef PIZZA_BMP_HEADER
#define PIZZA_BMP_HEADER

#include <cstdint>

#pragma pack(push, 1) // Need to add so the program reads headers properly
struct BMPFileHeader {
  uint16_t signature{0x4D42}; // Should be "BM" in hex which is 0x4D42
  uint32_t fileSize{0};
  uint16_t reserved1{0};
  uint16_t reserved2{0};
  uint32_t dataPosition{0};
};

struct BMPDIBHeader {
  uint32_t headerSize{0};
  int32_t width{0};
  int32_t height{0};
  uint16_t planes{1};
  uint16_t bitsPerPixel{0};
  uint32_t compressionType{0};
  uint32_t imageSize{0};
  int32_t horizontalResolution{0};
  int32_t verticalResolution{0};
  uint32_t numberOfColorsInTable{0};
  uint8_t colorsImportant{0};
  uint8_t colorsRotation{0};
  uint16_t reserved{0};
};

#pragma pack(pop)

#endif