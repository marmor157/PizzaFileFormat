#include "Algs.h"
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <map>

/**
 * @brief converts RGB value to grayscale
 *
 * @param color
 * @return uint8_t
 */
uint8_t RGBtoGrayscale(Color color) {
  return 0.299 * color.r + 0.587 * color.g + 0.114 * color.b;
}

/**
 * @brief Get the Difference In Colors object values
 *
 * @param color1
 * @param color2
 * @return int
 */
int getDifferenceInColors(Color color1, Color color2) {
  return abs(color1.r - color2.r) + abs(color1.g - color2.g) +
         abs(color1.b - color2.b);
}

/**
 * @brief Returs index of Color closest to given one in given Color Table
 *
 * @param color
 * @param colorTable
 * @param colorTableSize
 * @return int
 */
int findClosestColorIndexFromTable(Color color, std::vector<Color> colorTable,
                                   int colorTableSize, bool toGrayscale) {
  int retVal = 0;

  int minDifference = getDifferenceInColors(color, colorTable[0]);

  int difference;

  if (toGrayscale) {
    color.r = RGBtoGrayscale(color);
    color.b = color.g = color.r;
  }

  for (int i = 1; i < colorTableSize; i++) {
    difference = getDifferenceInColors(color, colorTable[i]);

    if (difference < minDifference) {
      retVal = i;
      minDifference = difference;
    }
  }
  return retVal;
}

/**
 * @brief Generates custom 6 bit color table for best image reproduction
 *
 * @param image Two dimensional array of Colors, image[width][height]
 * @param imageWidth
 * @param imageHeight
 * @return std::vector<Color>
 */
std::vector<Color> generate6BitColorTable(BMP &bmp) {
  std::vector<Color> retVal;
  std::vector<int> occurrences;

  for (int i = 0; i < bmp.getWidth(); ++i) {
    for (int j = 0; j < bmp.getHeight(); ++j) {
      auto index = std::find(retVal.begin(), retVal.end(), bmp.getPixel(i, j)) -
                   retVal.begin();
      if (index != retVal.size())
        occurrences[index]++;
      else {
        retVal.push_back(bmp.getPixel(i, j));
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
      // occurrences[color1Pos] += occurrences[color2Pos];
      retVal.erase(color2);
    } else {
      // occurrences[color2Pos] += occurrences[color1Pos];
      retVal.erase(color1);
    }
    minDifference = 768;
  }

  return retVal;
}

/**
 * @brief returns compressed image using LZW compression
 *
 * @param image Two dimensional array of Colors, image[width][height]
 * @param imageWidth
 * @param imageHeight
 * @return std::list<int>
 */
std::list<int> generateLZWCompressedImage(uint8_t **image, int imageWidth,
                                          int imageHeight) {
  std::map<std::string, int> dictionary;

  // Initializing dictionary with values from 0 to 64, 6bit
  int dictSize = 64;
  for (int i = 0; i < 64; ++i) {
    dictionary[std::string(1, i)] = i;
  }

  // Converting two dimenssional table to string
  std::string uncompressed;
  for (int i = 0; i < imageWidth; ++i) {
    for (int j = 0; j < imageHeight; ++j) {
      uncompressed += image[i][j];
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

/**
 * @brief decompresses image from LZW compression
 *
 * @param compressed
 * @return std::string
 */
std::string decompressLZWImage(std::list<int> compressed) {
  std::map<int, std::string> dictionary;

  // Initializing dictionary with values from 0 to 64, 6bit
  int dictSize = 64;
  for (int i = 0; i < 64; ++i) {
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
      throw std::runtime_error("Bad compression of data");

    retVal += entry;
    dictionary[dictSize++] = previous + entry[0];

    previous = entry;
  }

  return retVal;
}

/**
 * @brief Converts decompressed string to 2D array of Colors
 *
 * @param input
 * @param image Two dimensional array of Colors, image[width][height]
 * @param imageWidth
 * @param imageHeight
 */
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

/**
 * @brief Get the Minimum Number Of Bits per value needed to save given data
 *
 * @param data
 * @return uint8_t
 */
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

/**
 * @brief copies color table to another
 *
 * @param source
 * @param destination
 */
void copyColorTableToVector(const Color *source,
                            std::vector<Color> &destination) {
  destination.insert(destination.begin(), source, source + 64);
  // std::copy(source, source + 64, destination);
}

void writeBit(std::fstream &file, int bit, bool force) {
  static int current_bit = 8;
  static unsigned char bit_buffer;

  if (force) {
    current_bit = 1;
  }

  if (bit)
    bit_buffer |= (1 << current_bit - 1);

  --current_bit;

  if (current_bit == 0) {
    file.write((char *)&bit_buffer, 1);
    current_bit = 8;
    bit_buffer = 0;
  }
}

std::vector<Color> sortColorsBySubcolor(std::vector<Color> &colors,
                                        char subcolor) {
  if (subcolor == 'r') {
    std::sort(colors.begin(), colors.end(),
              [](const Color &lhs, const Color &rhs) { return lhs.r > rhs.r; });
  } else if (subcolor == 'g') {
    std::sort(colors.begin(), colors.end(),
              [](const Color &lhs, const Color &rhs) { return lhs.g > rhs.g; });
  } else {
    std::sort(colors.begin(), colors.end(),
              [](const Color &lhs, const Color &rhs) { return lhs.b > rhs.b; });
  }

  return colors;
}

char findBiggestRange(std::vector<Color> colors) {
  int rMin = 256, rMax = 0, gMin = 256, gMax = 0, bMin = 256, bMax = 0;

  for (auto it = colors.begin(); it != colors.end(); ++it) {
    rMin = std::min(rMin, (int)it->r);
    rMax = std::max(rMax, (int)it->r);
    gMin = std::min(gMin, (int)it->g);
    gMax = std::max(gMax, (int)it->g);
    bMin = std::min(bMin, (int)it->b);
    bMax = std::max(bMax, (int)it->b);
  }

  const int rRange = rMax - rMin;
  const int gRange = gMax - gMin;
  const int bRange = bMax - bMin;

  const int biggestRange = std::max(rRange, std::max(gRange, bRange));

  if (biggestRange == rRange)
    return 'r';
  else if (biggestRange == gRange)
    return 'g';
  else
    return 'b';
}

std::vector<Color> generate6BitColorTableMedianCut(BMP &bmp) {
  std::vector<Color> retVal;

  for (int i = 0; i < bmp.getWidth(); ++i) {
    for (int j = 0; j < bmp.getHeight(); ++j) {
      auto index = std::find(retVal.begin(), retVal.end(), bmp.getPixel(i, j));
      if (index == retVal.end())
        retVal.push_back(bmp.getPixel(i, j));
    }
  }

  return medianCut(retVal, 0);
}

std::vector<Color> medianCut(std::vector<Color> colors, int depth,
                             int maxDepth) {
  if (depth == maxDepth) {
    int r = 0, g = 0, b = 0;
    for (auto it = colors.begin(); it != colors.end(); ++it) {
      r += it->r;
      g += it->g;
      b += it->b;
    }
    r = r / colors.size();
    g = g / colors.size();
    b = b / colors.size();
    return std::vector<Color>(1, Color{r, g, b});
  }

  const char subcolorToSortBy = findBiggestRange(colors);
  sortColorsBySubcolor(colors, subcolorToSortBy);

  std::size_t const half_size = colors.size() / 2;
  std::vector<Color> left =
      std::vector<Color>(colors.begin(), colors.begin() + half_size);
  std::vector<Color> right =
      std::vector<Color>(colors.begin() + half_size, colors.end());

  left = medianCut(left, depth + 1, maxDepth);
  right = medianCut(right, depth + 1, maxDepth);

  left.insert(left.end(), right.begin(), right.end());
  return left;
}