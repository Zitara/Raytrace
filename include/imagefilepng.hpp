#ifndef IMAGEFILEPNG_H
#define IMAGEFILEPNG_H

#include <fstream>
#include <png.h>
#include <iostream>

class imageFilepng
{
//private:


public:
    unsigned int width = 0;
    unsigned int height = 0;
    png_byte color_type = PNG_COLOR_TYPE_RGB;
    png_byte bit_depth = 8;
    png_bytep *row_pointers = nullptr;

    imageFilepng();
    imageFilepng(unsigned int image_width, unsigned int image_height);
    void read_png_file(char *filename);
    void write_png_file(char *filename);

};

#endif // IMAGEFILEPNG_H
