#ifndef PIZZA_PIZZA
#define PIZZA_PIZZA
#include "BMP.h"
#include "Color.h"
#include "DefaultColorTable.h"
#include "DefaultGrayscaleTable.h"
#include "PizzaHeader.h"
#include <fstream>
#include <string>

class BMP;

class Pizza {
public:
  PizzaHeader m_header;
  Color *m_colorTable;
  uint8_t **m_pixels;
  int m_width;
  int m_height;

public:
  Pizza(int width, int height, int colorTable = 0);
  Pizza(std::string name);
  Pizza(BMP &bmp, int colorTable = 1);
  void loadFromFile(std::string name);
  void saveToFile(std::string name);
  int getWidth() { return m_width; }
  int getHeight() { return m_height; }
  Color getPixel(int x, int y) { return m_colorTable[m_pixels[x][y]]; };
  ~Pizza();
};

#endif