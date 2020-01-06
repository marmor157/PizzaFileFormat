#ifndef PIZZA_ALGORITHMS
#define PIZZA_ALGORITHMS
#include <cmath>
#include <cstdint>
#include <fstream>
#include <list>
#include <string>
#include <vector>

#include "BMP.h"
#include "Color.h"

uint8_t RGBtoGrayscale(Color color);

int findClosestColorIndexFromTable(Color color, std::vector<Color> colorTable,
                                   int colorTableSize, bool toGrayscale = 0);

std::vector<Color> generate6BitColorTable(BMP &bmp);

std::list<int> generateLZWCompressedImage(uint8_t **image, int imageWidth,
                                          int imageHeight);

std::string decompressLZWImage(std::list<int> compressed);

void convertStringToColor(std::string input, Color **image, int imageWidth,
                          int imageHeight);

uint8_t getMinimumNumberOfBits(std::list<int> data);

void copyColorTableToVector(const Color *source,
                            std::vector<Color> &destination);

void writeBit(std::fstream &file, int bit, bool force = 0);

#endif