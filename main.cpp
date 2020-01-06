#include "Algs.h"
#include "BMP.h"
#include "Pizza.h"
#include <iostream>

using namespace std;

int main() {
  BMP test("obrazek2a.bmp");
  // Pizza pizza(test, 0);
  // Pizza pizza2(test, 1);
  Pizza pizza3(test, 1);
  pizza3.saveToFile("obrazek33.pizza");
  Pizza tak("obrazek33.pizza");

  // BMP bmp(pizza);
  // BMP bmp2(pizza2);
  BMP bmp3(tak);
  // bmp.saveToFile("obrazek31.bmp");
  // bmp2.saveToFile("obrazek32.bmp");
  bmp3.saveToFile("obrazek32.bmp");

  /*std::list<int> compressed =
      generateLZWCompressedImage(test.m_pixels, test.m_width, test.m_height);

  std::string decompressed = decompressLZWImage(compressed);

  convertStringToColor(decompressed, test.m_pixels, test.m_width,
                       test.m_height);

  test.saveToFile("obrazek32.bmp");*/

  return 1;
}