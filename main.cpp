#include "Algs.h"
#include "BMP.h"
#include "Pizza.h"
#include <iostream>

using namespace std;

int main() {
  BMP test("obrazek4.bmp");
  Pizza pizza(test, 1);
  pizza.saveToFile("obrazek321.pizza");
  std::cout << pizza.getPixel(0, 0).r << std::endl;

  /*std::list<int> compressed =
      generateLZWCompressedImage(test.m_pixels, test.m_width, test.m_height);

  std::string decompressed = decompressLZWImage(compressed);

  convertStringToColor(decompressed, test.m_pixels, test.m_width,
                       test.m_height);

  test.saveToFile("obrazek32.bmp");*/

  return 1;
}