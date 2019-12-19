#include "Algs.h"

uint8_t RGBtoGrayscale(Color color) {
  return 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
}

int findClosestColorIndexFromTable(Color color, Color *colorTable,
                                   int colorTableSize) {
  int retVal = 0;

  int minDifference = abs(color.r - colorTable[0].r) +
                      abs(color.g - colorTable[0].g) +
                      abs(color.b - colorTable[0].b);

  int difference;

  for (int i = 1; i < colorTableSize; i++) {
    difference = abs(color.r - colorTable[i].r) +
                 abs(color.g - colorTable[i].g) +
                 abs(color.b - colorTable[i].b);

    if (difference < minDifference) {
      retVal = i;
      minDifference = difference;
    }
  }
  return retVal;
}