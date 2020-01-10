#include "Algs.h"
#include "BMP.h"
#include "Pizza.h"
#include <iostream>

using namespace std;

int main() {
  int conrersionType;
  string inputFile, outputFile;
  cout << "Choose conversion type" << endl;
  cout << "1. BMP -> Pizza" << endl;
  cout << "2. Pizza -> BMP" << endl;
  cin >> conrersionType;
  if (conrersionType == 1) {
    cout << "Insert input file name(with extension)" << endl;
    cin >> inputFile;

    BMP bmp(inputFile);

    int dithering, colorTableType, algType = 0;

    cout << "Choose option" << endl;
    cout << "0.Dithering OFF" << endl;
    cout << "1.Dithering ON" << endl;
    cin >> dithering;

    cout << "Choose color table type" << endl;
    cout << "0.Default Color" << endl;
    cout << "1.Default Grayscale" << endl;
    cout << "2.Dedicated Color" << endl;
    cin >> colorTableType;

    if (colorTableType == 2) {
      cout << "Choose color table converions type" << endl;
      cout << "0.Median Cut" << endl;
      cout << "1.Brute Force(can be very slow when image has a lot of colors)"
           << endl;
      cin >> algType;
    }

    Pizza pizza(bmp, colorTableType, dithering, algType);

    cout << "Insert output file name(with extenison)" << endl;
    cin >> outputFile;

    pizza.saveToFile(outputFile);
  } else {
    cout << "Insert input file name(with extension)" << endl;
    cin >> inputFile;

    Pizza pizza(inputFile);
    BMP bmp(pizza);

    cout << "Insert output file name(with extenison)" << endl;
    cin >> outputFile;

    bmp.saveToFile(outputFile);
  }

  return 1;
}