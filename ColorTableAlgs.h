#ifndef PIZZA_COLOR_ALGORITHMS
#define PIZZA_COLOR_ALGORITHMS
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <vector>

#include "Algs.h"
#include "BMP.h"
#include "Color.h"

int findClosestColorIndexFromTable(Color color, std::vector<Color> colorTable,
                                   bool toGrayscale = 0);

void copyColorTableToVector(const Color *source,
                            std::vector<Color> &destination);

std::vector<Color> generate6BitColorTable(BMP &bmp);

std::vector<Color> sortColorsBySubcolor(std::vector<Color> &colors,
                                        char subcolor);

char findBiggestRange(std::vector<Color> colors);

std::vector<Color> medianCut(std::vector<Color> colors, int depth,
                             int maxDepth = 6); // Default 64 colors

std::vector<Color> generate6BitColorTableMedianCut(BMP &bmp);

#endif