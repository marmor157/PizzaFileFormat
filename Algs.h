#ifndef PIZZA_ALGORITHMS
#define PIZZA_ALGORITHMS
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>
#include <vector>

#include "BMP.h"
#include "Color.h"
#include "ColorTableAlgs.h"

uint8_t RGBtoGrayscale(Color color);

std::list<int> generateLZWCompressedImage(uint8_t **image, int imageWidth,
                                          int imageHeight);

std::string decompressLZWImage(std::list<int> compressed);

uint8_t getMinimumNumberOfBits(std::list<int> data);

void writeBit(std::fstream &file, int bit, bool force = 0);

void applyDithering(BMP &bmp, std::vector<Color> &colorTable);

#endif