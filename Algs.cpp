#include "Algs.h"

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
 * @brief Stores bits and saves to file when whole byte is ready
 *
 * @param file file to save to
 * @param bit current bit value
 * @param force if given saves current bits, rest of byte is filled with 0
 */
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

/**
 * @brief Applies dithering for given bmp image and given color table
 *
 * @param bmp
 * @param colorTable
 */
void applyDithering(BMP &bmp, std::vector<Color> &colorTable) {
  Color currentPixel;
  Color choosenColor;

  // Storage for errors in color conversion
  float **errorsR = new float *[bmp.getWidth() + 2];
  float **errorsG = new float *[bmp.getWidth() + 2];
  float **errorsB = new float *[bmp.getWidth() + 2];
  for (int i = 0; i < bmp.getWidth() + 2; ++i) {
    errorsR[i] = new float[bmp.getHeight() + 2]{0};
    errorsG[i] = new float[bmp.getHeight() + 2]{0};
    errorsB[i] = new float[bmp.getHeight() + 2]{0};
  }

  int shift = 1; // To not exceed table range
  float currentErrorR = 0, currentErrorG = 0, currentErrorB = 0;

  for (int y = 0; y < bmp.getHeight(); y++)
    for (int x = 0; x < bmp.getWidth(); x++) {
      currentPixel = bmp.getPixel(x, y);

      // Passing current error to current Pixel with overflow protetcion
      if (currentPixel.r + errorsR[x + shift][y] > 255)
        currentPixel.r = 255;
      else if (currentPixel.r + errorsR[x + shift][y] < 0)
        currentPixel.r = 0;
      else
        currentPixel.r += errorsR[x + shift][y];

      if (currentPixel.g + errorsG[x + shift][y] > 255)
        currentPixel.g = 255;
      else if (currentPixel.r + errorsG[x + shift][y] < 0)
        currentPixel.g = 0;
      else
        currentPixel.g += errorsG[x + shift][y];

      if (currentPixel.b + errorsB[x + shift][y] > 255)
        currentPixel.b = 255;
      else if (currentPixel.b + errorsB[x + shift][y] < 0)
        currentPixel.b = 0;
      else
        currentPixel.b += errorsB[x + shift][y];

      // Matching color from give table
      choosenColor =
          colorTable[findClosestColorIndexFromTable(currentPixel, colorTable)];

      // Calculating error from current pixel
      currentErrorR = currentPixel.r - choosenColor.r;
      currentErrorG = currentPixel.g - choosenColor.g;
      currentErrorB = currentPixel.b - choosenColor.b;

      bmp.setPixel(x, y, choosenColor);

      // Pasing errors from current pixels to rest as determined in
      // Floyd–Steinber alg
      errorsR[x + 1 + shift][y] += (currentErrorR * 7.0 / 16.0);
      errorsR[x + 1 + shift][y + 1] += (currentErrorR * 1.0 / 16.0);
      errorsR[x + shift][y + 1] += (currentErrorR * 5.0 / 16.0);
      errorsR[x - 1 + shift][y + 1] += (currentErrorR * 3.0 / 16.0);

      errorsG[x + 1 + shift][y] += (currentErrorG * 7.0 / 16.0);
      errorsG[x + 1 + shift][y + 1] += (currentErrorG * 1.0 / 16.0);
      errorsG[x + shift][y + 1] += (currentErrorG * 5.0 / 16.0);
      errorsG[x - 1 + shift][y + 1] += (currentErrorG * 3.0 / 16.0);

      errorsB[x + 1 + shift][y] += (currentErrorB * 7.0 / 16.0);
      errorsB[x + 1 + shift][y + 1] += (currentErrorB * 1.0 / 16.0);
      errorsB[x + shift][y + 1] += (currentErrorB * 5.0 / 16.0);
      errorsB[x - 1 + shift][y + 1] += (currentErrorB * 3.0 / 16.0);
    }
}