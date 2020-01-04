#include "Algs.h"
#include "BMP.h"
#include "Pizza.h"
#include <iostream>

using namespace std;

int main() {
  BMP test("obrazek2.bmp");
  Pizza pizza(test);
  pizza.saveToFile("obrazek321.pizza");
  Pizza pizza2("obrazek321.pizza");
  BMP test2(pizza2);
  test2.saveToFile("obrazek312.bmp");

  /*std::list<int> compressed =
      generateLZWCompressedImage(test.m_pixels, test.m_width, test.m_height);

  std::string decompressed = decompressLZWImage(compressed);

  convertStringToColor(decompressed, test.m_pixels, test.m_width,
                       test.m_height);

  test.saveToFile("obrazek32.bmp");*/

  return 1;
}