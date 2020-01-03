#ifndef PIZZA_ALGORITHMS
#define PIZZA_ALGORITHMS
#include <cmath>
#include <cstdint>
#include <fstream>
#include <list>
#include <string>
#include <vector>

#include "Color.h"

uint8_t RGBtoGrayscale(Color color);

int findClosestColorIndexFromTable(Color color, const Color *colorTable,
                                   int colorTableSize);

std::vector<Color> generate6BitColorTable(Color **image, int imageWidth,
                                          int imageHeight);

std::list<int> generateLZWCompressedImage(uint8_t **image, int imageWidth,
                                          int imageHeight);

std::string decompressLZWImage(std::list<int> compressed);

void convertStringToColor(std::string input, Color **image, int imageWidth,
                          int imageHeight);

uint8_t getMinimumNumberOfBits(std::list<int> data);

void copyColorTable(const Color *source, Color *destination);

void writeBit(std::fstream &file, int bit, bool force = 0);

#endif