#include "ColorTableAlgs.h"

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
                                   bool toGrayscale) {
  int retVal = 0;

  int minDifference = getDifferenceInColors(color, colorTable[0]);

  int difference;

  if (toGrayscale) {
    color.r = RGBtoGrayscale(color);
    color.b = color.g = color.r;
  }

  for (int i = 1; i < colorTable.size(); i++) {
    difference = getDifferenceInColors(color, colorTable[i]);

    if (difference < minDifference) {
      retVal = i;
      minDifference = difference;
    }
  }
  return retVal;
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
 * @brief Sorts vector of colors by given parameter
 *
 * @param colors Vector of Colors
 * @param subcolor r-red, g-green, b-blue
 * @return std::vector<Color> vector of sorted colors
 */
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

/**
 * @brief Finds in which Colors paramter there's the biggest difference
 *
 * @param colors vector of colors
 * @return char  r-red, g-green, b-blue
 */
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

/**
 * @brief Generates 2^maxDepth colors most matching for given vectors of colors
 *
 * @param colors vector of colors
 * @param depth current depth of algorithm
 * @param maxDepth max depth of algorithm
 * @return std::vector<Color> vector of 2^maxDepth
 */
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
    return std::vector<Color>(1, Color{(uint8_t)r, (uint8_t)g, (uint8_t)b});
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

/**
 * @brief Generates 64 colors most matching given bmp using median cut alg
 *
 * @param bmp
 * @return std::vector<Color> vector of 64 colors
 */
std::vector<Color> generate6BitColorTableMedianCut(BMP &bmp) {
  std::vector<Color> retVal;

  for (int i = 0; i < bmp.getWidth(); ++i) {
    for (int j = 0; j < bmp.getHeight(); ++j) {
      auto index = std::find(retVal.begin(), retVal.end(), bmp.getPixel(i, j));
      if (index == retVal.end())
        retVal.push_back(bmp.getPixel(i, j));
    }
  }
  if (retVal.size() <= 64)
    return retVal;
  else
    return medianCut(retVal, 0);
}
