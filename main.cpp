#include <iostream>

#include "BMP.h"

using namespace std;

int main() {
  BMP test("obrazek3.bmp");
  test.saveToFile("obrazek32.bmp");

  return 1;
}