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

  if (m_header.signature != "PIZZA")
    return;

  m_width = m_header.width;
  m_height = m_header.height;

  int colorTableType = m_header.colorTableAndCRC >> 6;

  if (colorTableType >= 2) {
    m_colorTable = new Color[64];

    unsigned char colorTableChar[192]; // 64 color, 24 bit for one
    file.read((char *)&colorTableChar, 192);

    for (int i = 0; i < 64; ++i) {
      m_colorTable[i] = Color{colorTableChar[i * 3], colorTableChar[i * 3 + 1],
                              colorTableChar[i * 3 + 2]};
    }

    file.seekg(204, file.beg); // Skip to pixel table
  } else {
    file.seekg(12, file.beg); // Skip to pixel table
    if (colorTableType == 0)  // Set default color table as current
      std::copy(std::begin(DEFAULT_COLOR_TABLE), std::end(DEFAULT_COLOR_TABLE),
                m_colorTable);
    else // Set default grayscale table as current
      std::copy(std::begin(DEFAULT_GRAYSCALE_TABLE),
                std::end(DEFAULT_GRAYSCALE_TABLE), m_colorTable);
  }

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
    // Reading pixels from bottom left corner
    file.read((char *)data, size); // read data every line, reading whole file
                                   // could couse problems with largers images

    for (int j = 0; j < m_width; ++j) {
      // in order: blue, green, red (little endian)
      // getting 2 bits for every color
      b = (data[(j * 6) / 8] >> (j * 6) % 8) & 0x02;
      g = (data[(j * 6 + 2) / 8] >> (j * 6 + 2) % 8) & 0x02;
      r = (data[(j * 6 + 4) / 8] >> (j * 6 + 4) % 8) & 0x02;
      // Get matching color from color table
      m_pixels[j][i] = m_colorTable[16 * r + 4 * g + b];
    }
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

Pizza::~Pizza() {
  delete[] m_colorTable;
  for (int i = 0; i < m_width; ++i) {
    delete[] m_pixels[i];
  }
  delete[] m_pixels;
}