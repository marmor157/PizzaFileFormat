#include "Algs.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>

uint8_t RGBtoGrayscale(Color color) {
  return 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
}

int getDifferenceInColors(Color color1, Color color2) {
  return abs(color1.r - color2.r) + abs(color1.g - color2.g) +
         abs(color1.b - color2.b);
}

int findClosestColorIndexFromTable(Color color, const Color *colorTable,
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

std::list<int> generateLZWCompressedImage(Color **image, int imageWidth,
                                          int imageHeight) {
  std::map<std::string, int> dictionary;

  // Initializing dictionary with values from 0 to 255
  int dictSize = 256;
  for (int i = 0; i < 256; ++i) {
    dictionary[std::string(1, i)] = i;
  }

  // Converting two dimenssional table to string
  std::string uncompressed;
  for (int i = 0; i < imageWidth; ++i) {
    for (int j = 0; j < imageHeight; ++j) {
      uncompressed += image[i][j].toString();
    }
  }
  // delcaration of variable used in algorithm
  std::string previous;
  char current;
  std::string pc; // previous + current;
  std::list<int> retVal;

  for (auto it = uncompressed.begin(); it != uncompressed.end(); ++it) {
    current = *it;
    pc = previous + current;
    if (dictionary.count(pc))
      previous = pc;
    else {
      retVal.push_back(dictionary[previous]);
      dictionary[pc] = dictSize++;
      previous = std::string(1, current);
    }
  }

  // If something is left add it to return value
  if (!previous.empty())
    retVal.push_back(dictionary[previous]);

  return retVal;
}

std::string decompressLZWImage(std::list<int> compressed) {
  std::map<int, std::string> dictionary;

  // Initializing dictionary with values from 0 to 255
  int dictSize = 256;
  for (int i = 0; i < 256; ++i) {
    dictionary[i] = std::string(1, i);
  }

  // Declaration of variables used in algorithm
  std::string previous(1, *compressed.begin());
  std::string retVal = previous;
  std::string entry;
  int current;

  for (auto it = ++(compressed.begin()); it != compressed.end(); ++it) {
    current = *it;

    if (dictionary.count(current))
      entry = dictionary[current];
    else if (current == dictSize) {
      entry = previous + previous[0];
    } else
      throw "Bad compressed list";

    retVal += entry;
    dictionary[dictSize++] = previous + entry[0];

    previous = entry;
  }

  return retVal;
}

void convertStringToColor(std::string input, Color **image, int imageWidth,
                          int imageHeight) {

  std::string::iterator current = input.begin();

  for (int i = 0; i < imageWidth; ++i) {
    for (int j = 0; j < imageHeight; ++j) {
      image[i][j].r = (uint8_t)*current++;
      image[i][j].g = (uint8_t)*current++;
      image[i][j].b = (uint8_t)*current++;
    }
  }
}

uint8_t getMinimumNumberOfBits(std::list<int> data) {
  uint8_t retVal = 0;

  int currentBits = 0;

  for (auto it = data.begin(); it != data.end(); ++it) {
    *it >>= 8; // values up to 255 are default so we can skip that
    while (*it > 0) {
      *it >>= 1;
      ++currentBits;
    }
    if (currentBits > retVal)
      retVal = currentBits;
    currentBits = 0;
  }

  return retVal + 8;
}