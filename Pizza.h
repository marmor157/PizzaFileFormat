#ifndef PIZZA_PIZZA
#define PIZZA_PIZZA
#include "BMP.h"
#include "Color.h"
#include "PizzaHeader.h"
#include <fstream>
#include <string>

class Pizza {
  PizzaHeader m_header;
  Color *m_colorTable;
  Color **m_pixels;
  int m_width;
  int m_height;

public:
  Pizza(int width, int height);
  Pizza(std::string name);
  Pizza(BMP bmp);
};

#endif