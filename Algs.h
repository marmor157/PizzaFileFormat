#ifndef PIZZA_ALGORITHMS
#define PIZZA_ALGORITHMS
#include <cmath>
#include <cstdint>
#include <string>
#include <vector>


#include "Color.h"

uint8_t RGBtoGrayscale(Color color);

int findClosestColorIndexFromTable(Color color, Color *colorTable,
                                   int colorTableSize);

std::vector<Color> generate6BitColorTable(Color **image, int imageWidth,
                                          int imageHeight);

std::string generateLZWCompressedImage(Color **image, int imageWidth,
                                       int imageHeight);

#endif