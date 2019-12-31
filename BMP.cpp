#include "BMP.h"

#include <exception>
#include <iostream>

BMP::BMP(uint32_t width, uint32_t height)
    : m_width(width), m_height(height), m_pixels(new Color *[width]) {
  for (int i = 0; 0 < width; ++i) {
    m_pixels[i] = new Color[height];
  }
};

BMP::BMP(std::string name) { loadFromFile(name); }

BMP::BMP(Pizza pizza) {
  m_width = pizza.getWidth();
  m_height = pizza.getHeight();

  m_pixels = new Color *[m_width];
  for (int i = 0; i < m_width; ++i) {
    m_pixels[i] = new Color[m_height];
    for (int j = 0; j < m_height; ++j) {
      m_pixels[i][j] = pizza.getPixel(i, j);
    }
  }
}

void BMP::loadFromFile(std::string name) {
  std::fstream file;
  file.open(name, std::ios::in | std::ios::binary);

  // Read file info
  file.read((char *)&m_fileHeader, 14);

  if (m_fileHeader.signature != 0x4D42) {
    throw std::runtime_error("Error! Unrecognized file format.");
  }

  file.read((char *)&m_DIBHeader, sizeof(m_DIBHeader));

  m_width = m_DIBHeader.width;
  m_height = m_DIBHeader.height;

  // skip to start of pixer array
  file.seekg(*(char *)&m_fileHeader.dataPosition, file.beg);

  // Declaring pixels array
  m_pixels = new Color *[m_width];
  for (int i = 0; i < m_width; i++) {
    m_pixels[i] = new Color[m_height];
  }

  // Getting pixels from file
  int size = m_DIBHeader.bitsPerPixel / 8 * m_width;
  unsigned char *data = new unsigned char[size];

  uint8_t r, g, b;

  // Copying pixel data from file to Pixels array
  for (int i = m_height - 1; i >= 0; --i) {
    // BMP saves pixels from the bottom left, so do i
    file.read((char *)data, size); // read data every line, reading whole file
                                   // would couse problems with largers images

    for (int j = 0; j < m_width; ++j) {
      // BMP saves in order: blue, green, red
      b = *(data++);
      g = *(data++);
      r = *(data++);
      if (m_DIBHeader.bitsPerPixel == 32) // If alpha channel exists, skip it
        data++;
      m_pixels[j][i] = {r, g, b};
    }
    data -= size; // resets pointer to point on the start of the array
  }

  delete[] data;
  file.close();
}

// Saving int to char, little endian safe
void saveIntToChar(unsigned char *a, int number) {
  a[0] = number & 0xff;
  a[1] = (number >> 8) & 0xff;
  a[2] = (number >> 16) & 0xff;
  a[3] = (number >> 24) & 0xff;
}

void BMP::saveToFile(std::string name) {
  std::fstream file;
  file.open(name, std::ios::out | std::ios::binary);

  m_DIBHeader.headerSize = 40; // Setting basic DIB header size
  m_fileHeader.dataPosition =
      14 + m_DIBHeader.headerSize; // File header always takes 14 bytes
  m_fileHeader.fileSize = m_width * m_height * 3 + m_fileHeader.dataPosition;

  m_DIBHeader.width = m_width;
  m_DIBHeader.height = m_height;
  m_DIBHeader.bitsPerPixel = 24; // Saving with aplha channel not supported
  m_DIBHeader.imageSize = m_height * m_width * 3;

  file.write((char *)&m_fileHeader, 14); // Saving file header to file
  file.write((char *)&m_DIBHeader,
             m_DIBHeader.headerSize); // saving DIB header to file

  int size = 3 * m_width;
  unsigned char *data = new unsigned char[size];

  // Saving rom bottom row to the first
  for (int i = m_height - 1; i >= 0; i--) {
    for (int j = 0; j < m_width; j++) {
      // save in reverse order, little endian
      data[j * 3] = (char)m_pixels[j][i].b;
      data[j * 3 + 1] = (char)m_pixels[j][i].g;
      data[j * 3 + 2] = (char)m_pixels[j][i].r;
    }
    file.write((char *)data, size);
  }

  delete[] data;
  file.close();
}

Color BMP::getPixel(int x, int y) {
  if (x < m_width && y < m_height)
    return m_pixels[x][y];
}

BMP::~BMP() {
  for (int i = 0; i < m_width; ++i) {
    delete[] m_pixels[i];
  }
  delete m_pixels;
}