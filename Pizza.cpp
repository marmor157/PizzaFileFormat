#include "Pizza.h"

Pizza::Pizza(int width, int height)
    : m_width(width), m_height(height), m_pixels(new Color *[width]) {
  for (int i = 0; i < width; ++i) {
    m_pixels[i] = new Color[height];
  }
}

Pizza::Pizza(std::string name) {
  std::fstream file;
  file.open(name, std::ios::in | std::ios::binary);

  file.read((char *)&m_header, 11);

  if (m_header.magicValue != "PIZZA")
    return;

  m_width = m_header.width;
  m_height = m_header.height;
  // Checking if BMP is in 32-bit per color mode

  if (m_header.colorTableAndCRC & 0b11000000 >= 2)
    file.seekg(60, file.beg);
  else
    file.seekg(12, file.beg);

  // Declaring pixels array
  m_pixels = new Color *[m_width];
  for (int i = 0; i < m_width; i++) {
    m_pixels[i] = new Color[m_height];
  }

  // Getting pixels from file
  int size = m_header.LZWWordLength * m_width;
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
      m_pixels[j][i] = {r, g, b};
    }
    data -= size; // resets pointer to point on the start of the array
  }

  delete[] data;
  file.close();
}

Pizza::Pizza(BMP bmp) {
  m_width = bmp.getWidth();
  m_height = bmp.getHeight();

  m_pixels = new Color *[m_width];
  for (int i = 0; i < m_width; ++i) {
    m_pixels[i] = new Color[m_height];
    for (int j = 0; j < m_height; ++j) {
      m_pixels[i][j] = bmp.getPixel(i, j);
    }
  }
}