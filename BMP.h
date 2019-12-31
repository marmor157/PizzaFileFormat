#ifndef PIZZA_BMP
#define PIZZA_BMP

#include <fstream>
#include <string>

#include "BMPHeaders.h"
#include "Color.h"
#include "Pizza.h"

class Pizza;

class BMP {
public:
  uint32_t m_width;
  uint32_t m_height;
  Color **m_pixels;
  BMPFileHeader m_fileHeader;
  BMPDIBHeader m_DIBHeader;

public:
  BMP(uint32_t width, uint32_t height);
  BMP(std::string name);
  BMP(Pizza pizza);
  void loadFromFile(std::string name);
  void saveToFile(std::string name);
  uint32_t getWidth() { return m_width; }
  uint32_t getHeight() { return m_height; }
  Color getPixel(int i, int j);
  ~BMP();
};

#endif