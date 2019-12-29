#include "Algs.h"
#include "BMP.h"
#include "Pizza.h"
#include <iostream>

using namespace std;

int main() {
  BMP test("MARBLES.BMP");
  test.saveToFile("obrazek32.bmp");

  Pizza testPizza(test);
  std::vector<int> compressed = generateLZWCompressedImage(
      testPizza.m_pixels, testPizza.m_width, testPizza.m_height);

  convertStringToColor(decompressLZWImage(compressed), testPizza.m_pixels,
                       testPizza.m_width, testPizza.m_height);

  test = BMP(testPizza);
  test.saveToFile("obrazek32.bmp");

  return 1;
}