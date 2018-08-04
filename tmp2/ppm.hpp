#ifndef PPMHPP
#define PPMHPP


/* Using non-compressed image file format PPM.
 * Supplying a header file of a simple C++ class that allows read/write
 * PPM images and store them in memory as three arrays (for the R,G,B
 * colors) of unsigned characters.
 *
 * from: https://solarianprogrammer.com/2011/12/16/cpp-11-thread-tutorial/
 *
 * Code structure to parallilise the workload:
 *  - load image to memory.
 *  - Split image into max number of system threads.
 *  - Launch (number of threads - 1), each processing his chunk of image.
 *  - main() thread deal with the last chunk of the image.
 *  - Wait and join() threads to main() thread when done.
 *  - Save processed image.
 *
 */

#include <iostream>
#include <vector>

class ppm{
  /// bool flag_alloc;
  void init();
  // info about the PPM file (height and width)
  unsigned int nr_lines;
  unsigned int nr_columns;

  public:
    // arrays for storing R,G,B values
    std::vector<unsigned char> r;
    std::vector<unsigned char> g;
    std::vector<unsigned char> b;

    unsigned int height;
    unsigned int width;
    unsigned int max_col_val;

    // total number of elements (pixels)
    unsigned int size;

    ppm();
    // create a PPM object and fill it with data stored fname
    ppm(const std::string &fname);
    // create an "empty" PPM image with a given width and height
    // R,G,B arrays are filled with zeros
    ppm(const unsigned int _width, const unsigned int _height);
    // free memory used by the R,G,B vectors when object is destroyed
    /// ~ppm();
    // read the PPM image from fname
    void read(const std::string &fname);
    // write the PPM image in fname
    void write(const std::string &fname);
};


#endif
