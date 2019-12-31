#include "Algs.h"
#include "BMP.h"
#include "Pizza.h"
#include <iostream>

using namespace std;

int main() {
  BMP test("obrazek3.bmp");
  cout << (int)test.getPixel(25, 3).r << std::endl;
  test.saveToFile("obrazek321.bmp");
  cout << (int)test.getPixel(25, 3).g << std::endl;
  std::list<int> compressed =
      generateLZWCompressedImage(test.m_pixels, test.m_width, test.m_height);

  std::string decompressed = decompressLZWImage(compressed);

  convertStringToColor(decompressed, test.m_pixels, test.m_width,
                       test.m_height);
  cout << (int)test.getPixel(25, 3).r << std::endl;
  test.saveToFile("obrazek32.bmp");

  return 1;
}