#ifndef PIZZA_ALGORITHMS
#define PIZZA_ALGORITHMS
#include <cmath>
#include <cstdint>

#include "Color.h"

uint8_t RGBtoGrayscale(Color color);

int findClosestColorIndexFromTable(Color color, Color *colorTable,
                                   int colorTableSize);

#endif