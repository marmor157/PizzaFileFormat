#include "Algs.h"
#include <algorithm>
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
  std::vector<int> occurrences;

  for (int i = 0; i < imageWidth; ++i) {
    for (int j = 0; j < imageHeight; ++j) {
      auto index =
          std::find(retVal.begin(), retVal.end(), image[i][j]) - retVal.begin();
      if (index != retVal.size())
        occurrences[index]++;
      else {
        retVal.push_back(image[i][j]);
        occurrences.push_back(1);
      }
    }
  }

  int minDifference = 768;
  std::vector<Color>::iterator color1, color2;
  int color1Pos, color2Pos;

  while (retVal.size() > 64) {
    for (auto i = retVal.begin(); i != retVal.end(); i++) {
      for (auto j = std::next(i, 1); j != retVal.end(); j++) {
        int difference = getDifferenceInColors(*i, *j);
        if (difference < minDifference) {
          minDifference = difference;
          color1 = i;
          color2 = j;
        }
      }
    }
    color1Pos = color1 - retVal.begin();
    color2Pos = color2 - retVal.begin();

    if (occurrences[color1Pos] >= occurrences[color2Pos]) {
      occurrences[color1Pos] += occurrences[color2Pos];
      retVal.erase(color2);
    } else {
      occurrences[color2Pos] += occurrences[color1Pos];
      retVal.erase(color1);
    }
    minDifference = 768;
  }

  return retVal;
}