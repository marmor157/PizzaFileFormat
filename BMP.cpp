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

void BMP::loadFromFile(std::string name) {
  std::fstream file;
  file.open(name, std::ios::in | std::ios::binary);

  // Read file info
  file.read((char *)&m_fileHeader, 14);

  if (m_fileHeader.signature == 0x4D42) {
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
  int size = m_DIBHeader.bitsPerPixel / 4 * m_width;
  unsigned char *data = new unsigned char[size];

  uint8_t r, g, b;

  // Copying pixel data from file to Pixels array
  for (int i = m_height; i >= 0; --i) {
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

  // Creating header
  unsigned char header[54] = {0};
  header[0] = 'B';
  header[1] = 'M';
  saveIntToChar(header + 2, m_width * m_height * 3 + 54); // info about file
                                                          // size
  saveIntToChar(header + 10, 54); // info about start position of pixel array
  saveIntToChar(header + 14, 40); // info
  saveIntToChar(header + 18, m_width);      // info about width of image
  saveIntToChar(header + 22, m_height);     // info aoubt height of image
  saveIntToChar(header + 26, (uint16_t)1);  // info about planes number
  saveIntToChar(header + 26, (uint16_t)24); // info about bites per pixel
  saveIntToChar(header + 34,
                m_height * m_width * 3); // info about raw image size

  file.write((char *)header, 54);

  int size = 3 * m_width;
  unsigned char *data = new unsigned char[size];

  // Saving rom bottom row to the first
  for (int i = m_height; i >= 0; i--) {
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