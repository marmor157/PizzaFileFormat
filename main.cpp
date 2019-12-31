#include "Algs.h"
#include "BMP.h"
#include "Pizza.h"
#include <iostream>

using namespace std;

int main() {
  BMP test("obrazek3.bmp");
  test.saveToFile("obrazek321.bmp");

  // Pizza testPizza(test);
  std::list<int> compressed =
      generateLZWCompressedImage(test.m_pixels, test.m_width, test.m_height);

  cout << (int)(getMinimumNumberOfBits(compressed));

  std::string decompressed = decompressLZWImage(compressed);

  convertStringToColor(decompressed, test.m_pixels, test.m_width,
                       test.m_height);

  // test = BMP(testPizza);
  test.saveToFile("obrazek32.bmp");

  return 1;
}