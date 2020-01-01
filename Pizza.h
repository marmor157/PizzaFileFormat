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
  Color **m_pixels;
  int m_width;
  int m_height;

public:
  Pizza(int width, int height);
  Pizza(std::string name);
  Pizza(BMP bmp);
  void loadFromFile(std::string name);
  int getWidth() { return m_width; }
  int getHeight() { return m_height; }
  Color getPixel(int x, int y) { return m_pixels[x][y]; };
  void convertColorTo6bit();
  ~Pizza();
};

#endif