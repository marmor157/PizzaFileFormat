#include "Pizza.h"
#include "Algs.h"
#include "ColorTableAlgs.h"
#include <cmath>
#include <cstring>
#include <iostream>

/**
 * @brief Construct a new Pizza:: Pizza object
 *
 * @param width
 * @param height
 * @param colorTable 0=Color, 1=Grayscale, >1=Custom
 */
Pizza::Pizza(int width, int height, int colorTable)
    : m_width(width), m_height(height), m_pixels(new uint8_t *[width]) {
  for (int i = 0; i < width; ++i) {
    m_pixels[i] = new uint8_t[height];
  }

  m_header.colorTable = colorTable;

  m_colorTable.resize(64);
  if (colorTable)
    copyColorTableToVector(DEFAULT_GRAYSCALE_TABLE, m_colorTable);
  else
    copyColorTableToVector(DEFAULT_COLOR_TABLE, m_colorTable);
}

/**
 * @brief Construct a new Pizza:: Pizza object with loading from file initiated
 *
 * @param name File name
 */
Pizza::Pizza(std::string name) {
  loadFromFile(name);
  std::cout << "Szerokosc obrazka: " << m_header.width << std::endl;
  std::cout << "Wysokosc obrazka: " << m_header.height << std::endl;
}

/**
 * @brief Construct a new Pizza:: Pizza object with copying image from BMP
 *
 * @param bmp
 * @param colorTable 0=Color, 1=Grayscale, >1=Custom
 */
Pizza::Pizza(BMP &bmp, int colorTable, int isDitheringEnabled, int algType) {
  m_width = bmp.getWidth();
  m_height = bmp.getHeight();

  m_colorTable.resize(64);

  m_header.colorTable = colorTable;

  if (colorTable >= 2) {
    if (algType == 0)
      m_colorTable = generate6BitColorTableMedianCut(bmp);
    else
      m_colorTable = generate6BitColorTable(bmp);
  } else if (colorTable)
    copyColorTableToVector(DEFAULT_GRAYSCALE_TABLE, m_colorTable);
  else
    copyColorTableToVector(DEFAULT_COLOR_TABLE, m_colorTable);

  if (isDitheringEnabled)
    applyDithering(bmp, m_colorTable);

  m_pixels = new uint8_t *[m_width];
  for (int i = 0; i < m_width; ++i) {
    m_pixels[i] = new uint8_t[m_height];
    for (int j = 0; j < m_height; ++j) {
      m_pixels[i][j] = findClosestColorIndexFromTable(
          bmp.getPixel(i, j), m_colorTable, colorTable == 1);
    }
  }

  std::cout << "Szerokosc obrazka: " << m_width << std::endl;
  std::cout << "Wysokosc obrazka: " << m_height << std::endl;
  std::cout << "Typ palety koloru: ";
  if (colorTable == 0)
    std::cout << "Kolorowa domyslna" << std::endl;
  else if (colorTable == 1)
    std::cout << "Skala szarosci domyslna" << std::endl;
  else
    std::cout << "Kolorowa dedykowana" << std::endl;
}

/**
 * @brief loads image from file
 *
 * @param name fileName
 */
void Pizza::loadFromFile(std::string name) {
  std::fstream file;
  // We open the file at the end so we are able to determine its size
  file.open(name, std::ios::in | std::ios::binary | std::ios::ate);

  int dataSize = (int)file.tellg() - 8;

  // Aftrer reading the size, we have to reset position to the start
  file.seekg(0);

  file.read((char *)&m_header, 8);

  if (m_header.signature != 0x4950) {
    throw std::runtime_error("Error! Unrecognized file format.");
  }

  m_width = m_header.width;
  m_height = m_header.height;

  m_colorTable.resize(64);
  if (m_header.colorTable >= 2) {

    dataSize -= 192;

    unsigned char colorTableChar[192]; // 64 color, 24 bit for one
    file.read((char *)&colorTableChar, 192);

    for (int i = 0; i < 64; ++i) {
      m_colorTable[i] = Color{colorTableChar[i * 3], colorTableChar[i * 3 + 1],
                              colorTableChar[i * 3 + 2]};
    }

    file.seekg(200, file.beg); // Skip to pixel table
  } else {
    file.seekg(8, file.beg); // Skip to pixel table

    if (m_header.colorTable == 0) // Set default color table as current
      copyColorTableToVector(DEFAULT_COLOR_TABLE, m_colorTable);
    else // Set default grayscale table as current
      copyColorTableToVector(DEFAULT_GRAYSCALE_TABLE, m_colorTable);
  }

  // Declaring pixels array
  m_pixels = new uint8_t *[m_width];
  for (int i = 0; i < m_width; i++) {
    m_pixels[i] = new uint8_t[m_height];
  }

  // Getting pixels from file
  uint8_t *data = new uint8_t[dataSize];

  file.read((char *)data, dataSize);

  int bitsLeft = m_header.LZWWordLength; // Bits left to add in current symbol
  std::list<int> compressed(dataSize * 8 / m_header.LZWWordLength, 0);
  std::list<int>::iterator it = compressed.begin();

  for (int i = 0; i < dataSize; ++i) {
    if (bitsLeft >= 8) {          // If we need all 8 bits from current char
      bitsLeft -= 8;              // Substract if from current bits we need
      *it |= data[i] << bitsLeft; // Then move bits up match their place
    } else {                      // If we dont need whole char
      *it |= data[i] >> (8 - bitsLeft); // Save number of bits we need
      compressed.push_back(0);          // Add new element to list
      ++it;                             // Make it current element

      data[i] &=
          ((2 << (7 - bitsLeft)) - 1); // Clear bits takeen by previous symbol

      bitsLeft = m_header.LZWWordLength - 8 +
                 bitsLeft; // Set new number of needed bits and substract left
                           // from current char
      *it |= data[i] << bitsLeft; // Save rest of bits from current char
    }
  }

  std::string decompressed = decompressLZWImage(compressed);

  std::string::iterator current = decompressed.begin();

  for (int i = 0; i < m_width; ++i) {
    for (int j = 0; j < m_height; ++j) {
      m_pixels[i][j] = (uint8_t)*current++;
    }
  }

  delete[] data;
  file.close();
}

/**
 * @brief Saves pizza to fiven file
 *
 * @param name File name
 */
void Pizza::saveToFile(std::string name) {
  std::fstream file;
  file.open(name, std::ios::out | std::ios::binary);

  m_header.width = m_width;
  m_header.height = m_height;

  std::list<int> compressed =
      generateLZWCompressedImage(m_pixels, m_width, m_height);

  m_header.LZWWordLength = getMinimumNumberOfBits(compressed);

  file.write((char *)&m_header, 8);

  // If custom color table is provided
  if (m_header.colorTable >= 2) {
    unsigned char data[192];
    for (int i = 0; i < 64; ++i) {
      data[i * 3] = (char)m_colorTable[i].r;
      data[i * 3 + 1] = (char)m_colorTable[i].g;
      data[i * 3 + 2] = (char)m_colorTable[i].b;
    }
    file.write((char *)&data, 192);
  }
  auto it = compressed.begin();
  for (auto it = compressed.begin(); it != compressed.end(); ++it) {
    for (int x = m_header.LZWWordLength - 1; x >= 0; --x) {
      writeBit(file, (*it >> x) & 0x01);
    }
  }

  writeBit(file, 0, 1); // Force write remaining bits
  file.close();
}

/**
 * @brief Destroy the Pizza:: Pizza object
 *
 */
Pizza::~Pizza() {
  for (int i = 0; i < m_width; ++i) {
    delete[] m_pixels[i];
  }
  delete[] m_pixels;
}