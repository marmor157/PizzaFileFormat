#include "Algs.h"
#include <map>

uint8_t RGBtoGrayscale(Color color) {
  return 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
}

int getDifferenceInColors(Color color1, Color color2) {
  return abs(color1.r - color2.r) + abs(color1.g - color2.g) +
         abs(color1.b - color2.b);
}

int findClosestColorIndexFromTable(Color color, Color *colorTable,
                                   int colorTableSize) {
  int retVal = 0;

  int minDifference = getDifferenceInColors(color, colorTable[0]);

  int difference;

  for (int i = 1; i < colorTableSize; i++) {
    difference = getDifferenceInColors(color, colorTable[i]);

    if (difference < minDifference) {
      retVal = i;
      minDifference = difference;
    }
  }
  return retVal;
}

std::vector<Color> generate6BitColorTable(Color **image, int imageWidth,
                                          int imageHeight) {
  std::vector<Color> retVal;
  retVal.reserve(64);

  std::map<Color, int> colors;

  for (int i = 0; i < imageWidth; ++i) {
    for (int j = 0; j < imageHeight; ++j) {
      auto index = colors.find(image[i][j]);
      if (index != colors.end())
        index->second++;
      else
        colors.insert(std::make_pair(image[i][j], 1));
    }
  }

  int minDifference = 768;
  std::map<Color, int>::iterator color1, color2;

  while (colors.size() > 64) {
    for (auto i = colors.begin(); i != colors.end(); i++) {
      for (auto j = std::next(i, 1); j != colors.end(); j++) {
        int difference = getDifferenceInColors(i->first, j->first);
        if (difference < minDifference) {
          minDifference = difference;
          color1 = i;
          color2 = j;
        }
      }
    }

    if (color1->second >= color2->second) {
      color1->second += color2->second;
      colors.erase(color2);
    } else {
      color2->second += color1->second;
      colors.erase(color1);
    }
    minDifference = 768;
  }

  int arrayIndex = 0;
  for (auto it = colors.begin(); it != colors.end(); it++) {
    retVal.push_back(Color(it->first));
  }

  return retVal;
}