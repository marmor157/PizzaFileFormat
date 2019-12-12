#include "BMP.h"
#include <iostream>

BMP::BMP(uint32_t width, uint32_t height) : m_width(width), m_height(height), m_pixels(new Color *[width])
{
	for (int i = 0; 0 < width; ++i)
	{
		m_pixels[i] = new Color[height];
	}
};

BMP::BMP(std::string name)
{
	load(name);
}

void BMP::load(std::string name)
{
	std::fstream file;
	file.open(name, std::ios::in | std::ios::binary);

	//Read header and DIB of BMP
	unsigned char info[54];
	file.read((char *)&info, 54);

	//Get width, height and start of pixel array from header
	m_width = *(int *)&info[18];
	m_height = *(int *)&info[22];
	//skip to start of pixer array
	file.seekg(*(char *)&info[10], file.beg);

	//Declaring pixels array
	m_pixels = new Color *[m_width];
	for (int i = 0; i < m_width; i++)
	{
		m_pixels[i] = new Color[m_height];
	}

	//Getting pixels from file
	int size = 3 * m_width;
	unsigned char *data = new unsigned char[size];

	uint32_t r, g, b;

	//Copying pixel data from file to Pixels array
	for (int i = m_height; i >= 0; --i)
	{																 //BMP saves pixels from the bottom left, so do i
		file.read((char *)data, size); //read data every line, reading whole file would couse problems with largers images

		for (int j = 0; j < m_width; ++j)
		{
			//BMP saves in order: blue, green, red
			b = *(data++);
			g = *(data++);
			r = *(data++);
			m_pixels[j][i] = {r, g, b};
		}
		data -= size; //resets pointer to point on the start of the array
	}

	delete[] data;
}